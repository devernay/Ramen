// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_IMAGE_SSE_PROJECTIVE_WARP_HPP
#define RAMEN_IMAGE_SSE_PROJECTIVE_WARP_HPP

#include<OpenEXR/ImathBox.h>
#include<OpenEXR/ImathMatrix.h>

#include<ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{
namespace sse
{

void projective_warp_bilinear( const Imath::Box2i& src_area, const const_image_view_t& src,
		     const Imath::Box2i& dst_area, const image_view_t& dst,
		     const Imath::M33d& xform, const Imath::M33d& inv_xform);

void projective_warp_bicubic( const Imath::Box2i& src_area, const const_image_view_t& src,
		     const Imath::Box2i& dst_area, const image_view_t& dst,
		     const Imath::M33d& xform, const Imath::M33d& inv_xform);

} // namespace
} // namespace
} // namespace

#endif

