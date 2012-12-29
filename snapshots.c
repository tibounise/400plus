/**
 * $Revision$
 * $Date$
 * $Author$
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>

#include "main.h"
#include "firmware.h"

#include "cmodes.h"
#include "display.h"
#include "languages.h"
#include "utils.h"

#include "snapshots.h"

int snapshot_read(char *name, snapshot_t *snapshot) {
	int result  = false;
	int file    = -1;
	int version =  0;

	snapshot_t buffer;

	if ((file = FIO_OpenFile(name, O_RDONLY, 644)) == -1)
		goto end;

	if (FIO_ReadFile(file, &version, sizeof(version)) != sizeof(version))
		goto end;

	if (version != SETTINGS_VERSION)
		goto end;

	if (FIO_ReadFile(file, &buffer, sizeof(buffer)) != sizeof(buffer))
		goto end;

	*snapshot = buffer;

	result = true;

end:
	if (file != -1)
		FIO_CloseFile(file);

	return result;
}

int snapshot_write(char *name) {
	const int version = SETTINGS_VERSION;

	int  result = false;
	int  file   = -1;

	snapshot_t buffer = {
		DPData     : DPData,
		settings   : settings,
		menu_order : menu_order,
	};

	if ((file = FIO_OpenFile(name, O_CREAT | O_WRONLY , 644)) == -1)
		goto end;

	if (FIO_WriteFile(file, (void*)&version, sizeof(version)) != sizeof(version))
		goto end;

	if (FIO_WriteFile(file, (void*)&buffer, sizeof(buffer)) != sizeof(buffer))
		goto end;

	if (FIO_CloseFile(file) == -1)
		goto end;

	result = true;

end:
	if (file != -1)
		FIO_CloseFile(file);

	return result;
}

int snapshot_delete(char *name) {
	return (FIO_RemoveFile(name) != -1);
}

void snapshot_recall(snapshot_t *snapshot) {
	send_to_intercom(IC_SET_AE, 1, snapshot->DPData.ae);
}

void snapshot_apply(snapshot_t *snapshot) {
	if (cmodes_config.recall_camera) {
		send_to_intercom(IC_SET_METERING,   1, snapshot->DPData.metering);
		send_to_intercom(IC_SET_EFCOMP,     1, snapshot->DPData.efcomp);
		send_to_intercom(IC_SET_DRIVE,      1, snapshot->DPData.drive);
		send_to_intercom(IC_SET_WB,         1, snapshot->DPData.wb);
		send_to_intercom(IC_SET_AF_POINT,   2, snapshot->DPData.af_point);
		send_to_intercom(IC_SET_TV_VAL,     1, snapshot->DPData.tv_val);
		send_to_intercom(IC_SET_AV_VAL,     1, snapshot->DPData.av_val);
		send_to_intercom(IC_SET_AV_COMP,    1, snapshot->DPData.av_comp);
		send_to_intercom(IC_SET_ISO,        2, snapshot->DPData.iso);
		send_to_intercom(IC_SET_RED_EYE,    1, snapshot->DPData.red_eye);
		send_to_intercom(IC_SET_AE_BKT,     1, snapshot->DPData.ae_bkt);
		send_to_intercom(IC_SET_WB_BKT,     1, snapshot->DPData.wb_bkt);
		send_to_intercom(IC_SET_BEEP,       1, snapshot->DPData.beep);
		send_to_intercom(IC_SET_COLOR_TEMP, 2, snapshot->DPData.color_temp);
		send_to_intercom(IC_SET_WBCOMP_GM,  1, snapshot->DPData.wbcomp_gm);
		send_to_intercom(IC_SET_WBCOMP_AB,  1, snapshot->DPData.wbcomp_ab);

		/**
		 *  We cannot switch AF off when loading a custom mode,
		 *  because the switch on the lens could be set to on.
		 */
		if (snapshot->DPData.af)
			send_to_intercom(IC_SET_AF, 1, snapshot->DPData.af);
	}

	if (cmodes_config.recall_settings) {
		send_to_intercom(IC_SET_AUTO_POWER_OFF, 2, snapshot->DPData.auto_power_off);
		send_to_intercom(IC_SET_VIEW_TYPE,      1, snapshot->DPData.view_type);
		send_to_intercom(IC_SET_REVIEW_TIME,    1, snapshot->DPData.review_time);
		send_to_intercom(IC_SET_AUTO_ROTATE,    1, snapshot->DPData.auto_rotate);
		send_to_intercom(IC_SET_LCD_BRIGHTNESS, 1, snapshot->DPData.lcd_brightness);
		send_to_intercom(IC_SET_DATE_TIME,      1, snapshot->DPData.date_time);
		send_to_intercom(IC_SET_FILE_NUMBERING, 1, snapshot->DPData.file_numbering);
		send_to_intercom(IC_SET_LANGUAGE,       1, snapshot->DPData.language);
		send_to_intercom(IC_SET_VIDEO_SYSTEM,   1, snapshot->DPData.video_system);
		send_to_intercom(IC_SET_HISTOGRAM,      1, snapshot->DPData.histogram);
		send_to_intercom(IC_SET_COLOR_SPACE,    1, snapshot->DPData.color_space);
	}

	if (cmodes_config.recall_image) {
		send_to_intercom(IC_SET_IMG_FORMAT,  1, snapshot->DPData.img_format);
		send_to_intercom(IC_SET_IMG_SIZE,    1, snapshot->DPData.img_size);
		send_to_intercom(IC_SET_IMG_QUALITY, 1, snapshot->DPData.img_quality);
	}

	if (cmodes_config.recall_cfn) {
		send_to_intercom(IC_SET_CF_SET_BUTTON_FUNC,      1, snapshot->DPData.cf_set_button_func);
		send_to_intercom(IC_SET_CF_NR_FOR_LONG_EXPOSURE, 1, snapshot->DPData.cf_nr_for_long_exposure);
		send_to_intercom(IC_SET_CF_EFAV_FIX_X,           1, snapshot->DPData.cf_efav_fix_x);
		send_to_intercom(IC_SET_CF_AFAEL_ACTIVE_BUTTON,  1, snapshot->DPData.cf_afael_active_button);
		send_to_intercom(IC_SET_CF_EMIT_AUX,             1, snapshot->DPData.cf_emit_aux);
		send_to_intercom(IC_SET_CF_EXPLEVEL_INC_THIRD,   1, snapshot->DPData.cf_explevel_inc_third);
		send_to_intercom(IC_SET_CF_EMIT_FLASH,           1, snapshot->DPData.cf_emit_flash);
		send_to_intercom(IC_SET_CF_EXTEND_ISO,           1, snapshot->DPData.cf_extend_iso);
		send_to_intercom(IC_SET_CF_AEB_SEQUENCE,         1, snapshot->DPData.cf_aeb_sequence);
		send_to_intercom(IC_SET_CF_SI_INDICATE,          1, snapshot->DPData.cf_si_indicate);
		send_to_intercom(IC_SET_CF_MENU_POS,             1, snapshot->DPData.cf_menu_pos);
		send_to_intercom(IC_SET_CF_MIRROR_UP_LOCK,       1, snapshot->DPData.cf_mirror_up_lock);
		send_to_intercom(IC_SET_CF_FPSEL_METHOD,         1, snapshot->DPData.cf_fpsel_method);
		send_to_intercom(IC_SET_CF_FLASH_METERING,       1, snapshot->DPData.cf_flash_metering);
		send_to_intercom(IC_SET_CF_FLASH_SYNC_REAR,      1, snapshot->DPData.cf_flash_sync_rear);
		send_to_intercom(IC_SET_CF_SAFETY_SHIFT,         1, snapshot->DPData.cf_safety_shift);
		send_to_intercom(IC_SET_CF_LENS_BUTTON,          1, snapshot->DPData.cf_lens_button);
		send_to_intercom(IC_SET_CF_ORIGINAL_EVAL,        1, snapshot->DPData.cf_original_eval);
		send_to_intercom(IC_SET_CF_QR_MAGNIFY,           1, snapshot->DPData.cf_qr_magnify);
		send_to_intercom(IC_SET_CF_TFT_ON_POWER_ON,      1, snapshot->DPData.cf_tft_on_power_on);
	}

	if (cmodes_config.recall_ordering) {
		menu_order = snapshot->menu_order;
	}

	if (cmodes_config.recall_400plus) {
		settings = snapshot->settings;
		settings_apply();
	}
}
