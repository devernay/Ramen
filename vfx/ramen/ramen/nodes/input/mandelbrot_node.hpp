// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_MANDELBROT_NODE_HPP
#define RAMEN_MANDELBROT_NODE_HPP

#include<ramen/nodes/base/generator_node.hpp>

namespace ramen
{

class mandelbrot_node_t : public generator_node_t
{
public:

    mandelbrot_node_t();

protected:

    mandelbrot_node_t( const mandelbrot_node_t& other) : generator_node_t( other) {}

private:

    virtual node_t *do_clone() const { return new mandelbrot_node_t( *this);}

    virtual void do_create_params();

    virtual void do_process( const render::render_context_t& context);

    friend class boost::serialization::access;

    void serialize( boost::archive::polymorphic_iarchive& ar, const unsigned int version);
    void serialize( boost::archive::polymorphic_oarchive& ar, const unsigned int version) const;
};

} // namespace

#endif
