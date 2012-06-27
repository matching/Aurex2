// $Id: config.h,v 1.1 2006/03/06 12:59:08 fumi Exp $

#ifndef CONFIG_H
#define CONFIG_H

#ifdef HAVE_SDCONFIG
# include "sdconfig.h"
#endif


/* 境界整列されていないアドレスからの多バイト転送に失敗する CPU では定義する
 * # 2バイト転送 → アドレスが偶数
 * # 4バイト転送 → アドレスが4の倍数
 */
// #define FAIL_NOT_ALIGNED_ADDRESS

/* キー離れたということが認識できないシステムである場合は定義する
 */
// #define KEY_WITHOUT_PUSH_UP

/* エミュレータを実行するCPUがリトルエンディアン */
// #define HOST_IS_LITTLE_ENDIAN

/* エミュレータを実行するCPUがビッグエンディアン */
// #define HOST_IS_BIG_ENDIAN

#if defined(HOST_IS_LITTLE_ENDIAN) && defined(HOST_IS_BIG_ENDIAN)
# error "HOST_IS_LITTLE_ENDIAN and HOST_IS_BIG_ENDIAN are defined"
#endif

/* 以下デフォルトオプションの設定 */

#ifndef OPTION_DEFAULT_WARN_ODD_ACCESS
# define OPTION_DEFAULT_WARN_ODD_ACCESS false
#endif

#ifndef OPTION_DEFAULT_SOUND
# define OPTION_DEFAULT_SOUND false
#endif

#ifndef OPTION_DEFAULT_CPU_TIMER
# define OPTION_DEFAULT_CPU_TIMER false
#endif

#ifndef OPTION_DEFAULT_CPU_SPEED
# define OPTION_DEFAULT_CPU_SPEED 100
#endif

#ifndef OPTION_DEFAULT_TIMER_SPEED
# define OPTION_DEFAULT_TIMER_SPEED 0
#endif

#ifndef OPTION_DEFAULT_AFTER_IMAGE
# define OPTION_DEFAULT_AFTER_IMAGE false
#endif

#ifndef OPTION_DEFAULT_ANTI_ALIAS
# define OPTION_DEFAULT_ANTI_ALIAS false
#endif

#ifndef OPTION_DEFAULT_EXPANTION
# define OPTION_DEFAULT_EXPANTION 1
#endif

#ifndef OPTION_DEFAULT_AI_VALUE
# define OPTION_DEFAULT_AI_VALUE 8
#endif

#ifndef OPTION_DEFAULT_USE_ISO8859_1
# define OPTION_DEFAULT_USE_ISO8859_1 false
#endif

#endif // CONFIG_H
