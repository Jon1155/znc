/*
 * Copyright (C) 2004-2012  See the AUTHORS file for details.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#pragma once

// Define DEBUG macro, otherwise tinyxml.h will try to define it itself.
#include <znc/ZNCDebug.h>

#ifdef __GNUC__
#  if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#    pragma GCC diagnostic push
#  endif
#  pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif

#include <znc/xml/ticpp.h>

#ifdef __GNUC__
#  if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#     pragma GCC diagnostic pop
#  endif
#else
// Can't pop with this old GCC, just try to restore sane defaults.
#  pragma GCC diagnostic warning "-Woverloaded-virtual"
#endif
