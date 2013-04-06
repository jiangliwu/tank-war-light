#ifndef __RES_MANAGE__TOOLS_H__
#define  __RES_MANAGE__TOOLS_H__

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   //win32键盘响应事件
#include <windows.h>
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)
#endif


static const char map_tmx_lvl1[]		= "map/lvl1.tmx";
static const char map_tmx_lvl2[]		= "map/lvl2.tmx";
static const char map_tmx_lvl3[]		= "map/lvl3.tmx";
static const char tank_player[]			= "images/tank.png";
static const char control_bg[]			= "images/control_bg.png";
static const char control_ce[]			= "images/cen.png";
static const char fire_normal[]			= "images/fire.png";
static const char fire_press[]			= "images/fire_press.png";
static const char fire_pariticl[]		= "images/fire_pariticl.png";
static const char menu_about[]			= "images/about.png";
static const char menu_about_[]			= "images/about_.png";
static const char menu_exit[]			= "images/exit.png";
static const char menu_exit_[]			= "images/exit_.png";
static const char menu_load[]			= "images/load.png";
static const char menu_load_[]			= "images/load_.png";
static const char menu_main[]			= "images/main.png";
static const char menu_menubg[]			= "images/menubg.png";
static const char menu_options[]		= "images/options.png";
static const char menu_options_[]		= "images/options_.png";
static const char snow[]				= "images/snow.png";
static const char menu_start[]			= "images/start.png";
static const char menu_start_[]			= "images/start_.png";
static const char pause_resume[]		= "images/pausemenuresume.png";
static const char pause_save[]			= "images/pausemenusave.png";
static const char pause_return[]		= "images/pausemenureturn.png";
static const char pause_bg[]			= "images/pausebg.png";
static const char fire_music[]			= "sound/fire.mp3";

const int mapWidth = 25;
const int mapHeight = 15;


//#define  IAMPHONE__			//如果需要控制杆 取消注释即可
#endif									   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   
										   