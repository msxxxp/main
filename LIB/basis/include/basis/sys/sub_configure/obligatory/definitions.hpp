﻿#ifndef BASIS_SYS_CONFIGURE_OBLIGATORY_DEFINITIONS_HPP_
#define BASIS_SYS_CONFIGURE_OBLIGATORY_DEFINITIONS_HPP_

#define UNUSED(val) ((void)val)

#define THIS_FILE        filename_only(__FILE__)
#define THIS_PLACE       THIS_FILE, __LINE__, __PRETTY_FUNCTION__
#define THIS_PLACE_SHORT THIS_FILE, __LINE__, __FUNCTION__

#endif
