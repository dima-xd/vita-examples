#include <iostream>
#include <string>
#include <codecvt>
#include <stdio.h>
#include <string>
#include <psp2/message_dialog.h>

void wait_dialog() {
    while (1) {
        SceCommonDialogStatus status = sceMsgDialogGetStatus();
        if (status == SCE_COMMON_DIALOG_STATUS_FINISHED) {
            sceMsgDialogTerm();
            break;
        }
    }
}

int init_msg_dialog(const char* fmt, ...) {
	va_list list;
	char msg[1024];

	va_start(list, fmt);
	vsnprintf(msg, sizeof(msg), fmt, list);
	va_end(list);

	SceMsgDialogUserMessageParam msg_param;
	sceClibMemset(&msg_param, 0, sizeof(msg_param));
	msg_param.buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_OK;
	msg_param.msg = (SceChar8*)msg;

	SceMsgDialogParam param;
	sceMsgDialogParamInit(&param);
	_sceCommonDialogSetMagicNumber(&param.commonParam);
	param.mode = SCE_MSG_DIALOG_MODE_USER_MSG;
	param.userMsgParam = &msg_param;

	return sceMsgDialogInit(&param);
}

int init_interactive_msg_dialog(const char *fmt, ...) {
    va_list list;
    char msg[1024];

    va_start(list, fmt);
    vsnprintf(msg, sizeof(msg), fmt, list);
    va_end(list);

    SceMsgDialogUserMessageParam msg_param;
    sceClibMemset(&msg_param, 0, sizeof(msg_param));
    msg_param.buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_YESNO;
    msg_param.msg = (SceChar8 *)msg;

    SceMsgDialogParam param;
    sceMsgDialogParamInit(&param);
    _sceCommonDialogSetMagicNumber(&param.commonParam);
    param.mode = SCE_MSG_DIALOG_MODE_USER_MSG;
    param.userMsgParam = &msg_param;

    return sceMsgDialogInit(&param);
}

int init_progressbar_dialog(const char *fmt, ...) {
    va_list list;
    char msg[1024];

    va_start(list, fmt);
    vsnprintf(msg, sizeof(msg), fmt, list);
    va_end(list);

    SceMsgDialogProgressBarParam msg_param;
    sceClibMemset(&msg_param, 0, sizeof(msg_param));
    msg_param.barType = SCE_MSG_DIALOG_PROGRESSBAR_TYPE_PERCENTAGE;
    msg_param.msg = (const SceChar8*)msg;

    SceMsgDialogParam param;
    sceMsgDialogParamInit(&param);
    param.mode = SCE_MSG_DIALOG_MODE_PROGRESS_BAR;
    param.progBarParam = &msg_param;

    return sceMsgDialogInit(&param);
}

int main(int argc, char *argv[]) {
	init_msg_dialog("Message Dialog");
    wait_dialog();

    init_interactive_msg_dialog("Interactive Message Dialog");
    wait_dialog();

    init_progressbar_dialog("Progress Message Dialog");
    wait_dialog();
    return 0;
}
