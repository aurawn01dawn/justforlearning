#ifndef CONFIG_H
#define CONFIG_H

#define COM_UPDATE_TIME 100 //界面数据显示的更新时间，单位 ms
#define LEFTDOWNWIDGET_UPDATE_TIME 100 //左下角控件栏显示的更新时间，单位 ms
#define DATA_UPDATE_TIME 100 //数据的更新时间，单位 ms
#define IMAGE_CACHE 1024000 //允许的最大图像缓存大小 单位KB
#define SCENE_SELECT_ICON_UPDATE_TIME 500 //场景切换按钮的图标更新时间，单位ms

#define CONVERT_UUID_TO_POINTER_TIME 500 //读取文件时将事件函数中存储的uuid转换成指针的等待时间，单位ms

#define VIEW_RECT_SIZE 0,0,1831,973 //展示窗口分辨率大小，分别为x,y,w,h

#endif // CONFIG_H
