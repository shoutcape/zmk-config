/* SPDX-License-Identifier: MIT */

#define DT_DRV_COMPAT zmk_behavior_rapidfire

#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/events/keycode_state_changed.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct rapidfire_config {
    uint32_t keycode;
    uint32_t tap_ms;
    uint32_t wait_ms;
};

struct rapidfire_data {
    const struct device *dev;
    struct k_mutex lock;
    struct k_work_delayable work;
    bool held;
    bool pressed;
    int64_t next_at;
};

static void rapidfire_tick(struct k_work *work) {
    struct rapidfire_data *data =
        CONTAINER_OF(k_work_delayable_from_work(work), struct rapidfire_data, work);
    const struct rapidfire_config *config = data->dev->config;

    k_mutex_lock(&data->lock, K_FOREVER);
    if (data->held) {
        int64_t now = k_uptime_get();
        // A callback queued before release/re-press must respect the new press's deadline.
        if (now >= data->next_at) {
            data->pressed = !data->pressed;
            raise_zmk_keycode_state_changed_from_encoded(config->keycode, data->pressed, now);
            data->next_at = k_uptime_get() +
                            (data->pressed ? config->tap_ms : config->wait_ms);
        }
        k_work_reschedule(&data->work, K_MSEC(data->next_at - k_uptime_get()));
    }
    k_mutex_unlock(&data->lock);
}

static int rapidfire_pressed(struct zmk_behavior_binding *binding,
                             struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    struct rapidfire_data *data = dev->data;
    const struct rapidfire_config *config = dev->config;

    k_mutex_lock(&data->lock, K_FOREVER);
    if (!data->held) {
        data->held = true;
        data->pressed = true;
        raise_zmk_keycode_state_changed_from_encoded(config->keycode, true, event.timestamp);
        data->next_at = k_uptime_get() + config->tap_ms;
        k_work_reschedule(&data->work, K_MSEC(config->tap_ms));
    }
    k_mutex_unlock(&data->lock);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int rapidfire_released(struct zmk_behavior_binding *binding,
                              struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    struct rapidfire_data *data = dev->data;
    const struct rapidfire_config *config = dev->config;

    // Serialize cancellation and the final key-up with any in-flight timer callback.
    k_mutex_lock(&data->lock, K_FOREVER);
    data->held = false;
    k_work_cancel_delayable(&data->work);
    if (data->pressed) {
        data->pressed = false;
        raise_zmk_keycode_state_changed_from_encoded(config->keycode, false, event.timestamp);
    }
    k_mutex_unlock(&data->lock);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int rapidfire_init(const struct device *dev) {
    struct rapidfire_data *data = dev->data;
    data->dev = dev;
    k_mutex_init(&data->lock);
    k_work_init_delayable(&data->work, rapidfire_tick);
    return 0;
}

static const struct behavior_driver_api rapidfire_api = {
    .locality = BEHAVIOR_LOCALITY_CENTRAL,
    .binding_pressed = rapidfire_pressed,
    .binding_released = rapidfire_released,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

#define RAPIDFIRE_INST(n)                                                                           \
    BUILD_ASSERT(DT_INST_PROP(n, tap_ms) > 0, "rapidfire tap-ms must be positive");                   \
    BUILD_ASSERT(DT_INST_PROP(n, wait_ms) > 0, "rapidfire wait-ms must be positive");                 \
    static struct rapidfire_data rapidfire_data_##n;                                                \
    static const struct rapidfire_config rapidfire_config_##n = {                                   \
        .keycode = DT_INST_PROP(n, keycode),                                                        \
        .tap_ms = DT_INST_PROP(n, tap_ms),                                                          \
        .wait_ms = DT_INST_PROP(n, wait_ms),                                                        \
    };                                                                                            \
    BEHAVIOR_DT_INST_DEFINE(n, rapidfire_init, NULL, &rapidfire_data_##n, &rapidfire_config_##n,      \
                            POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &rapidfire_api);

DT_INST_FOREACH_STATUS_OKAY(RAPIDFIRE_INST)

#endif
