/**
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include"bl_defocus_plugin.hpp"

#include<string.h>

#include<memory>

#ifdef _WINDOWS
#include<windows.h>
#endif

#ifdef __APPLE__
#include<OpenGL/gl.h>
#else
#include<GL/gl.h>
#endif

#include"ofxsImageEffect.h"
#include"ofxsMultiThread.h"

#include"bl_defocus_processor.hpp"

class bl_defocus_plugin : public OFX::ImageEffect
{
public:

    bl_defocus_plugin( OfxImageEffectHandle handle) : ImageEffect( handle)
    {
        // clips
	src_clip_ = fetchClip( "Source");
        msk_clip_ = fetchClip( "Mask");
	dst_clip_ = fetchClip( "Output");

        quality_ = fetchChoiceParam( "quality");
        blur_    = fetchDoubleParam( "blur");
        btheresh_= fetchDoubleParam( "thereshold");
        shape_   = fetchChoiceParam( "shape");
        rotate_  = fetchDoubleParam( "rotate");
        mask_    = fetchChoiceParam( "mask");
    }

    void getClipPreferences(OFX::ClipPreferencesSetter& clipPreferences)
    {
          clipPreferences.setOutputFrameVarying( true);
    }

    bool getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod)
    {
        rod = src_clip_->getRegionOfDefinition( args.time);
        return true;
    }

    virtual void render( const OFX::RenderArguments &args)
    {
        bl_defocus_processor processor( *this);

        std::auto_ptr<OFX::Image> src( src_clip_->fetchImage( args.time));
        std::auto_ptr<OFX::Image> dst( dst_clip_->fetchImage( args.time));
        std::auto_ptr<OFX::Image> msk;

        double blur; blur_->getValue( blur);

        if( blur == 0.0)
        {
            // nothing to do, copy src to dst and return
            int y1 = src->getBounds().y1;
            int y2 = src->getBounds().y2;
            int x1 = src->getBounds().x1;
            int x2 = src->getBounds().x2;

            for( int y = y1; y < y2; ++y)
                memcpy( dst->getPixelAddress( x1, y), src->getPixelAddress( x1, y), (x2 - x1) * 4 * sizeof( float));

            return;
        }

        processor.setSrcImg( src.get());
        processor.setDstImg( dst.get());

        int use_mask; mask_->getValue( use_mask);

        if( msk_clip_->isConnected() && ( use_mask != 0))
        {
            msk.reset( msk_clip_->fetchImage( args.time));
            processor.setMaskImg( msk.get(), use_mask);
        }

        double btheresh; btheresh_->getValue( btheresh);
        int shape; shape_->getValue( shape);
        double rot; rotate_->getValue( rot);
        int quality; quality_->getValue( quality);


        NodeDefocus node_params;
        node_params.bktype = ( shape != 0) ? shape + 2 : 0;
        node_params.bthresh = btheresh;
        node_params.fstop = 128;
        node_params.gamco = 0;
        node_params.maxblur = 0;
        node_params.no_zbuf = 1;
        node_params.rotation = rot;
        node_params.scale = blur * args.renderScale.x;
        node_params.samples = 16;

        switch( quality)
        {
        case 0:
            node_params.preview = 1;
        break;

        case 1:
            node_params.preview = 1;
            node_params.samples = 32;
        break;

        case 2:
            node_params.preview = 1;
            node_params.samples = 64;
        break;

        case 3:
            node_params.preview = 1;
            node_params.samples = 128;
        break;

        case 4:
            node_params.preview = 0;
        break;
        }

        processor.setNodeInfo( &node_params);
        processor.setRenderWindow( args.renderWindow);
        processor.process();
    }

private:

    // clips
    OFX::Clip *src_clip_;
    OFX::Clip *msk_clip_;
    OFX::Clip *dst_clip_;

    // params
    OFX::ChoiceParam *quality_;
    OFX::DoubleParam *blur_;
    OFX::DoubleParam *btheresh_;
    OFX::ChoiceParam *shape_;
    OFX::DoubleParam *rotate_;
    OFX::ChoiceParam *mask_;
};
    
void bl_defocus_plugin_factory::describe( OFX::ImageEffectDescriptor& desc)
{
      desc.setLabels( "Bl_Defocus", "Bl_Defocus", "Bl_Defocus");
      desc.setPluginGrouping( "Blender");

      desc.addSupportedContext( OFX::eContextGeneral);

      desc.setSupportsTiles( false);
      desc.setSupportsMultiResolution( true);
      desc.addSupportedBitDepth( OFX::eBitDepthFloat);
      desc.setSingleInstance( false);
      desc.setHostFrameThreading( false);
      desc.setTemporalClipAccess( false);
      desc.setRenderTwiceAlways( false);
      desc.setSupportsMultipleClipPARs( false);
}

void bl_defocus_plugin_factory::describeInContext( OFX::ImageEffectDescriptor& desc, OFX::ContextEnum context)
{
    OFX::ClipDescriptor *srcClip = desc.defineClip("Source");
    srcClip->addSupportedComponent( OFX::ePixelComponentRGBA);
    srcClip->setTemporalClipAccess( false);
    srcClip->setSupportsTiles( false);
    srcClip->setIsMask( false);

    OFX::ClipDescriptor *mskClip = desc.defineClip("Mask");
    mskClip->addSupportedComponent( OFX::ePixelComponentRGBA);
    mskClip->setTemporalClipAccess( false);
    mskClip->setSupportsTiles( false);
    mskClip->setIsMask( false);
    mskClip->setOptional( true);

    OFX::ClipDescriptor *dstClip = desc.defineClip("Output");
    dstClip->addSupportedComponent( OFX::ePixelComponentRGBA);
    dstClip->setSupportsTiles( false);

    // create the params
    OFX::PageParamDescriptor *page = desc.definePageParam( "controls");

    OFX::ChoiceParamDescriptor *quality = desc.defineChoiceParam( "quality");
    quality->setLabels( "Quality", "Quality", "Quality");
    quality->setScriptName( "quality");
    quality->appendOption( "Preview");
    quality->appendOption( "Low");
    quality->appendOption( "Medium");
    quality->appendOption( "High");
    quality->appendOption( "Full");
    quality->setDefault( 0);
    page->addChild( *quality);

    OFX::DoubleParamDescriptor *blur = desc.defineDoubleParam( "blur");
    blur->setLabels( "Blur", "Blur", "Blur");
    blur->setScriptName( "blur");
    blur->setRange( 0, 100);
    blur->setDefault( 0);
    blur->setIncrement( 0.5);
    page->addChild( *blur);

    OFX::DoubleParamDescriptor *btheresh = desc.defineDoubleParam( "thereshold");
    btheresh->setLabels( "Thereshold", "Thereshold", "Thereshold");
    btheresh->setScriptName( "thereshold");
    btheresh->setRange( 0, 100);
    btheresh->setDefault( 1);
    btheresh->setIncrement( 0.1);
    page->addChild( *btheresh);

    OFX::ChoiceParamDescriptor *shape = desc.defineChoiceParam( "shape");
    shape->setLabels( "Shape", "Shape", "Shape");
    shape->setScriptName( "shape");
    shape->appendOption( "Disk");
    shape->appendOption( "Triangle");
    shape->appendOption( "Square");
    shape->appendOption( "Pentagon");
    shape->appendOption( "Hexagon");
    shape->appendOption( "Heptagon");
    shape->appendOption( "Octagon");
    shape->setDefault( 0);
    page->addChild( *shape);

    OFX::DoubleParamDescriptor *rotate = desc.defineDoubleParam( "rotate");
    rotate->setLabels( "Rotate", "Rotate", "Rotate");
    rotate->setScriptName( "rotate");
    rotate->setRange( 0, 90);
    rotate->setDefault( 0);
    rotate->setIncrement( 0.5);
    page->addChild( *rotate);

    OFX::ChoiceParamDescriptor *mask = desc.defineChoiceParam( "mask");
    mask->setLabels( "Mask", "Mask", "Mask");
    mask->setScriptName( "mask");
    mask->appendOption( "Off");
    mask->appendOption( "Luminance");
    mask->appendOption( "Alpha");
    mask->setDefault( 0);
    page->addChild( *mask);
}

OFX::ImageEffect* bl_defocus_plugin_factory::createInstance( OfxImageEffectHandle handle, OFX::ContextEnum context)
{
    return new bl_defocus_plugin( handle);
}
