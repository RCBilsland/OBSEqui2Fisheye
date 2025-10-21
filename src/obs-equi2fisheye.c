#include <obs-module.h>
#include <util/platform.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-equi2fisheye", "en-US")

#define S_EFFECT_PATH "effects/equi2fisheye.effect"

struct equi2fish_filter {
	obs_source_t *context;
	gs_effect_t *effect;
	gs_eparam_t *param_fov;
	gs_eparam_t *param_pan;
	gs_eparam_t *param_tilt;
	gs_eparam_t *param_yaw;
	gs_eparam_t *param_aspect;
	gs_eparam_t *param_outside_alpha;
	float fov_deg;
	float pan_deg;
	float tilt_deg;
	float yaw_deg;
	float outside_alpha;
};

static const char *equi2fish_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return "Equirectangular to Fisheye (Metal)";
}

static void equi2fish_update(void *data, obs_data_t *settings)
{
	struct equi2fish_filter *filter = data;
	filter->fov_deg = (float)obs_data_get_double(settings, "fov");
	filter->pan_deg = (float)obs_data_get_double(settings, "pan");
	filter->tilt_deg = (float)obs_data_get_double(settings, "tilt");
	filter->yaw_deg = (float)obs_data_get_double(settings, "yaw");
	filter->outside_alpha = (float)obs_data_get_double(settings, "outside_alpha");

	if (!filter->effect) {
		char *path = obs_module_file(S_EFFECT_PATH);
		if (path) {
			filter->effect = gs_effect_create_from_file(path, NULL);
			bfree(path);
			if (filter->effect) {
				filter->param_fov = gs_effect_get_param_by_name(filter->effect, "fov_deg");
				filter->param_pan = gs_effect_get_param_by_name(filter->effect, "pan_deg");
				filter->param_tilt = gs_effect_get_param_by_name(filter->effect, "tilt_deg");
				filter->param_yaw = gs_effect_get_param_by_name(filter->effect, "yaw_deg");
				filter->param_aspect = gs_effect_get_param_by_name(filter->effect, "aspect");
				filter->param_outside_alpha = gs_effect_get_param_by_name(filter->effect, "outside_alpha");
			}
		}
	}
}

static void *equi2fish_create(obs_data_t *settings, obs_source_t *context)
{
	struct equi2fish_filter *filter = bzalloc(sizeof(*filter));
	filter->context = context;
	equi2fish_update(filter, settings);
	return filter;
}

static void equi2fish_destroy(void *data)
{
	struct equi2fish_filter *filter = data;
	if (filter->effect) {
		gs_effect_destroy(filter->effect);
		filter->effect = NULL;
	}
	bfree(filter);
}

static obs_properties_t *equi2fish_properties(void *data)
{
	UNUSED_PARAMETER(data);
	obs_properties_t *props = obs_properties_create();
	obs_properties_add_float_slider(props, "fov", "FoV (deg)", 1.0, 220.0, 0.1);
	obs_properties_add_float_slider(props, "pan", "Pan (deg)", -180.0, 180.0, 0.1);
	obs_properties_add_float_slider(props, "tilt", "Tilt (deg)", -90.0, 90.0, 0.1);
	obs_properties_add_float_slider(props, "yaw", "Yaw (deg)", -180.0, 180.0, 0.1);
	obs_properties_add_float_slider(props, "outside_alpha", "Outside Alpha (0..1)", 0.0, 1.0, 0.01);
	return props;
}

static void equi2fish_defaults(obs_data_t *settings)
{
	obs_data_set_default_double(settings, "fov", 180.0);
	obs_data_set_default_double(settings, "pan", 0.0);
	obs_data_set_default_double(settings, "tilt", 0.0);
	obs_data_set_default_double(settings, "yaw", 0.0);
	obs_data_set_default_double(settings, "outside_alpha", 0.0);
}

static void equi2fish_video_render(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);
	struct equi2fish_filter *filter = data;
	obs_source_t *target = obs_filter_get_target(filter->context);
	obs_source_t *parent = obs_filter_get_parent(filter->context);
	
	if (!target || !parent || !filter->effect)
		return;

	uint32_t width = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);
	if (width == 0 || height == 0)
		return;

	if (!obs_source_process_filter_begin(filter->context, GS_RGBA, OBS_ALLOW_DIRECT_RENDERING))
		return;

	const float aspect = (float)width / (float)height;

	// Set effect parameters
	gs_effect_set_float(filter->param_fov, filter->fov_deg);
	gs_effect_set_float(filter->param_pan, filter->pan_deg);
	gs_effect_set_float(filter->param_tilt, filter->tilt_deg);
	gs_effect_set_float(filter->param_yaw, filter->yaw_deg);
	gs_effect_set_float(filter->param_aspect, aspect);
	gs_effect_set_float(filter->param_outside_alpha, filter->outside_alpha);

	// Render with our effect
	obs_source_process_filter_end(filter->context, filter->effect, width, height);
}

static uint32_t equi2fish_width(void *data)
{
	struct equi2fish_filter *filter = data;
	obs_source_t *parent = obs_filter_get_parent(filter->context);
	return parent ? obs_source_get_base_width(parent) : 0;
}

static uint32_t equi2fish_height(void *data)
{
	struct equi2fish_filter *filter = data;
	obs_source_t *parent = obs_filter_get_parent(filter->context);
	return parent ? obs_source_get_base_height(parent) : 0;
}

static struct obs_source_info equi2fish_filter_info = {
	.id = "equi2fisheye_filter",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_VIDEO,
	.get_name = equi2fish_get_name,
	.create = equi2fish_create,
	.destroy = equi2fish_destroy,
	.update = equi2fish_update,
	.get_defaults = equi2fish_defaults,
	.get_properties = equi2fish_properties,
	.video_render = equi2fish_video_render,
	.get_width = equi2fish_width,
	.get_height = equi2fish_height,
};

bool obs_module_load(void)
{
	obs_register_source(&equi2fish_filter_info);
	blog(LOG_INFO, "obs-equi2fisheye loaded");
	return true;
}

void obs_module_unload(void)
{
}
