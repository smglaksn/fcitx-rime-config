#pragma once
#include <libintl.h>

#define _(x) QString::fromUtf8(dgettext("fcitx-rime", x))
