/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.44 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIConf.h
Purpose     : Configures emWins abilities, fonts etc.
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *                      http://www.st.com/SLA0044
  *
  ******************************************************************************
  */
 
#ifndef GUICONF_H
#define GUICONF_H

/*********************************************************************
*
*       Multi layer/display support
*/
#define GUI_NUM_LAYERS            2    // Maximum number of available layers 最大可用层数 

/*********************************************************************
*
*       Multi tasking support
*/
#ifdef OS_SUPPORT                       //是否支持操作系统，C++选项卡中Define
 #define GUI_OS                    (1)  // Compile with multitasking support
#else
 #define GUI_OS                    (0)
#endif

/*********************************************************************
*
*       Configuration of touch support
*/
#ifndef   GUI_SUPPORT_TOUCH
  #define GUI_SUPPORT_TOUCH       (0)  // Support touchscreen
#endif

/*********************************************************************
*
*       Default font
*/
#define GUI_DEFAULT_FONT          &GUI_Font6x8   //默认字体

/*********************************************************************
*
*         Configuration of available packages
*/
#define GUI_SUPPORT_MOUSE             (0)    /* Support a mouse 是否启用鼠标 */   
#define GUI_WINSUPPORT                (1)    /* Use window manager 是否使用窗口管理器 */
#define GUI_SUPPORT_MEMDEV            (1)    /* Memory device package available 是否使用内存设备 */
#define GUI_SUPPORT_DEVICES           (1)    /* Enable use of device pointers 是否使用设备指针 */

#endif  /* Avoid multiple inclusion */
