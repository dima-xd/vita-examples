#include <iostream>
#include <string>
#include <cstring>
#include <codecvt>
#include <stdio.h>
#include <string>
#include <locale>
#include <psp2/message_dialog.h>
#include <psp2/ime_dialog.h>

static uint16_t dialog_res_text[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];
static bool is_ime_active = false;
static char text[128] = {0};

void getDialogTextResult(char *text) {
    std::u16string utf16_str = (char16_t*)dialog_res_text;
    std::string utf8_str = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(utf16_str.data());
    strcpy(text, utf8_str.c_str());
}

void wait_dialog() {
    while (1) {
        SceCommonDialogStatus status = sceMsgDialogGetStatus();
        if (status == SCE_COMMON_DIALOG_STATUS_FINISHED) {
            sceMsgDialogTerm();
            break;
        }
    }
}

void wait_ime_dialog() {
    if (is_ime_active) {
        while (1) {
            SceImeDialogResult res;
            sceClibMemset(&res, 0, sizeof(SceImeDialogResult));
            sceImeDialogGetResult(&res);

            if (res.button == SCE_IME_DIALOG_BUTTON_ENTER) {
                getDialogTextResult(text);
                sceImeDialogTerm();
                is_ime_active = false;
                break;
            }
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

void init_interactive_ime_dialog(const char *msg, const char *start_text, bool multiline) {
    SceImeDialogParam params;

    sceImeDialogParamInit(&params);
    params.type = SCE_IME_TYPE_BASIC_LATIN;

    std::string utf8_str = msg;
    std::u16string utf16_str = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(utf8_str.data());
    std::string utf8_arg = start_text;
    std::u16string utf16_arg = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(utf8_arg.data());

    params.title = (const SceWChar16*)utf16_str.c_str();
    sceClibMemset(dialog_res_text, 0, sizeof(dialog_res_text));
    sceClibMemcpy(dialog_res_text, utf16_arg.c_str(), utf16_arg.length() * 2);
    params.initialText = dialog_res_text;
    params.inputTextBuffer = dialog_res_text;

    params.maxTextLength = 20;

    if (multiline)
        params.option = SCE_IME_OPTION_MULTILINE;

    sceImeDialogInit(&params);
    is_ime_active = true;
}

int main(int argc, char *argv[]) {
	init_msg_dialog("Message Dialog");
    wait_dialog();

    init_interactive_msg_dialog("Interactive Message Dialog");
    wait_dialog();

    /*init_progressbar_dialog("Progress Message Dialog");
    wait_dialog();*/

    init_interactive_ime_dialog("IME Dialog", text, true);
    wait_ime_dialog();
    printf(text);

    init_interactive_ime_dialog("IME Dialog", text, false);
    wait_ime_dialog();
    printf(text);
    return 0;
}
