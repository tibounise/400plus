#include "main.h"
#include "settings.h"
#include "display.h"
#include "firmware.h"

#include "presets.h"

type_CAMERA_MODE preset;

void get_filename(char *filename, int id);

int preset_read(int id) {
	int result  = FALSE;

	int file    = -1;
	int version =  0;

	type_SETTINGS buffer;

	char filename[16];

	get_filename(filename, id);

	if ((file = FIO_OpenFile(filename, O_RDONLY, 644)) == -1)
		goto end;

	if (FIO_ReadFile(file, &version, sizeof(version)) != sizeof(version))
		goto end;

	if (version != SETTINGS_VERSION)
		goto end;

	if (FIO_ReadFile(file, &buffer, sizeof(buffer)) != sizeof(buffer))
		goto end;

	if (FIO_ReadFile(file, &preset, sizeof(preset)) != sizeof(preset))
		goto end;

	settings = buffer;
	result   = TRUE;

end:
	if (file != -1)
		FIO_CloseFile(file);

	return result;
}

void preset_write(int id) {
	const int version = SETTINGS_VERSION;

	int  file;
	char filename[16];

	get_filename(filename, id);
	file = FIO_OpenFile(filename, O_CREAT | O_WRONLY , 644);

	if (file != -1) {
		FIO_WriteFile(file, (void*)&version, sizeof(version));
		FIO_WriteFile(file, &settings,   sizeof(settings));
		FIO_WriteFile(file, &cameraMode, sizeof(cameraMode));
		FIO_CloseFile(file);
	}
}

extern void preset_apply() {
	settings_apply();

	SleepTask(10); SendToIntercom(EVENT_SET_AE,         1, preset.ae);
	SleepTask(10); SendToIntercom(EVENT_SET_METERING,   1, preset.metering);
	SleepTask(10); SendToIntercom(EVENT_SET_EFCOMP,     1, preset.efcomp);
	SleepTask(10); SendToIntercom(EVENT_SET_DRIVE,      1, preset.drive);
	SleepTask(10); SendToIntercom(EVENT_SET_WB,         1, preset.wb);
	SleepTask(10); SendToIntercom(EVENT_SET_AF,         1, preset.af);
	SleepTask(10); SendToIntercom(EVENT_SET_AF_POINT,   2, preset.af_point);
	SleepTask(10); SendToIntercom(EVENT_SET_TV_VAL,     1, preset.tv_val);
	SleepTask(10); SendToIntercom(EVENT_SET_AV_VAL,     1, preset.av_val);
	SleepTask(10); SendToIntercom(EVENT_SET_AV_COMP,    1, preset.av_comp);
	SleepTask(10); SendToIntercom(EVENT_SET_ISO,        2, preset.iso);
	SleepTask(10); SendToIntercom(EVENT_SET_RED_EYE,    1, preset.red_eye);
	SleepTask(10); SendToIntercom(EVENT_SET_AE_BKT,     1, preset.ae_bkt);
	SleepTask(10); SendToIntercom(EVENT_SET_WB_BKT,     1, preset.wb_bkt);
	SleepTask(10); SendToIntercom(EVENT_SET_BEEP,       1, preset.beep);
	SleepTask(10); SendToIntercom(EVENT_SET_COLOR_TEMP, 2, preset.color_temp);

	SleepTask(10); SendToIntercom(EVENT_SET_IMG_FORMAT,  1, preset.img_format);
	SleepTask(10); SendToIntercom(EVENT_SET_IMG_SIZE,    1, preset.img_size);
	SleepTask(10); SendToIntercom(EVENT_SET_IMG_QUALITY, 1, preset.img_quality);

	SleepTask(10); SendToIntercom(EVENT_SET_CF_SET_BUTTON_FUNC,      1, preset.cf_set_button_func);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_NR_FOR_LONG_EXPOSURE, 1, preset.cf_nr_for_long_exposure);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_EFAV_FIX_X,           1, preset.cf_efav_fix_x);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_AFAEL_ACTIVE_BUTTON,  1, preset.cf_afael_active_button);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_EMIT_AUX,             1, preset.cf_emit_aux);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_EXPLEVEL_INC_THIRD,   1, preset.cf_explevel_inc_third);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_EMIT_FLASH,           1, preset.cf_emit_flash);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_EXTEND_ISO,           1, preset.cf_extend_iso);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_AEB_SEQUENCE,         1, preset.cf_aeb_sequence);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_SI_INDICATE,          1, preset.cf_si_indicate);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_MENU_POS,             1, preset.cf_menu_pos);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_MIRROR_UP_LOCK,       1, preset.cf_mirror_up_lock);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_FPSEL_METHOD,         1, preset.cf_fpsel_method);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_FLASH_METERING,       1, preset.cf_flash_metering);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_FLASH_SYNC_REAR,      1, preset.cf_flash_sync_rear);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_SAFETY_SHIFT,         1, preset.cf_safety_shift);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_LENS_BUTTON,          1, preset.cf_lens_button);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_ORIGINAL_EVAL,        1, preset.cf_original_eval);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_QR_MAGNIFY,           1, preset.cf_qr_magnify);
	SleepTask(10); SendToIntercom(EVENT_SET_CF_TFT_ON_POWER_ON,      1, preset.cf_tft_on_power_on);

	display_refresh();
}

void get_filename(char *filename, int id) {
	sprintf(filename, PRESET_FILE, id);
}