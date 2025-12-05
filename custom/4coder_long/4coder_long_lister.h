/* date = March 3rd 2024 3:02 pm */

#ifndef FCODER_LONG_LISTER_H
#define FCODER_LONG_LISTER_H

enum Long_Lister_Type
{
    Long_Lister_Header = 1 << 0,
    Long_Lister_Slider = 1 << 1,
    Long_Lister_Switch = 1 << 2,
};

enum Long_Item_Type
{
    Long_Item_None,
    Long_Item_Config,
    Long_Item_Pointer,
    Long_Item_Attachment,
    Long_Item_ViewSetting,
};

struct Long_Lister_Data
{
    Long_Lister_Type type;
    Long_Item_Type item_type;
    Range_i64 range;
    u64 id;
    u64 user;
    String8 tooltip;
};

enum Long_Lister_TooltipType
{
    Long_Tooltip_UnderItem  = 0x80000000,
    Long_Tooltip_NextToItem = 0x80000001,
    //Long_Tooltip_TopLayout  = 0x80000002,
    //Long_Tooltip_BotLayout  = 0x80000003,
};

global i32 long_lister_tooltip_peek = 0;

function Long_Lister_Data* Long_Lister_AddItem(Lister* lister, String8 name, String8 tag);
function void Long_Lister_AddItem(Lister* lister, String8 name, String8 tag,
                                  Buffer_ID buffer, i64 pos, u64 index = 0, String8 tooltip = {});
function void Long_Lister_AddSwitch(Lister* lister, String8 name, Long_Item_Type type, u64 user = 0);
function void Long_Lister_AddSlider(Lister* lister, String8 name, Range_i64 range);

function Lister_Activation_Code Long_Lister_WriteString(Application_Links* app);
function void Long_Lister_Backspace(Application_Links* app);
function void Long_Lister_Backspace_Path(Application_Links* app);
function void Long_Lister_Navigate(Application_Links* app, View_ID view, Lister* lister, i32 delta);
function void Long_Lister_Navigate_NoSelect(Application_Links* app, View_ID view, Lister* lister, i32 delta);

function void    Long_Lister_Render(Application_Links* app, Frame_Info frame, View_ID view);
function void Long_Lister_RenderHUD(Long_Render_Context* ctx, Lister* lister, b32 show_file_bar);
function Lister_Result Long_Lister_Run(Application_Links* app, Lister* lister, b32 auto_select_first = 1);

#define Long_Lister_IsResultValid(result) (!(result).canceled && (result).user_data)

#endif //4CODER_LONG_LISTER_H
