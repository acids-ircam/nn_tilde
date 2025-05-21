#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include "ext_atomarray.h"                  // atomarrays

#include "../../../shared/static_buffer.h"

#include <type_traits>


namespace ArrayTools {

    namespace min = c74::min; 
    namespace max = c74::max;

    extern "C" max::t_atomarray* arrayobj_findregistered_retain(max::t_symbol* name);
    extern "C" max::t_max_err arrayobj_release(max::t_atomarray* aa);
    // extern void atomarray_dispose(max::t_atomarray* x);
    // extern max::t_atomarray* arrayobj_register(max::t_atomarray* aa, max::t_symbol** name);
    // extern max::t_max_err arrayobj_unregister(max::t_atomarray* aa);
    // extern max::t_atomarray* arrayobj_findregistered_clone(max::t_symbol* name);
    // extern max::t_symbol* arrayobj_namefromptr(max::t_atomarray* aa);
    // extern void* max::outlet_array(max::t_outlet* x, max::t_symbol* s);


    bool is_array(const min::atom &atom) {
        bool result = false;
        auto name_max = max::atom_getsym(&atom); 
        max::t_atomarray* aa = arrayobj_findregistered_retain(name_max);
        if (aa) {
            arrayobj_release(aa);
            return true;
        }
        return false;
    }

    long get_length(const min::atom &atom) {
        bool result = false;
        auto name_max = max::atom_getsym(&atom); 
        max::t_atomarray* aa = arrayobj_findregistered_retain(name_max);
        if (aa) {
            long size = atomarray_getsize(aa); 
            arrayobj_release(aa);
            return size;
        }
        return -1; 
    }

    void fill_long_vector(std::vector<long> &array, const min::atom &atom) {
        auto name_max = max::atom_getsym(&atom); 
        max::t_atomarray* aa = arrayobj_findregistered_retain(name_max);
        if (aa) {
            if (array.size() != 0) {
                throw "array not empty";
            }
            max::t_atomarray* clone = (max::t_atomarray*)object_clone((max::t_object*)aa); // CLONE, do not potentially modify upstream data
            max::t_atom atom_elt; 
            for (long i = 0; i < max::atomarray_getsize(clone); i++) {
                max::atomarray_getindex(clone, i, &atom_elt);                
                array.emplace_back(max::atom_getlong(&atom_elt)); 
            }
            arrayobj_release(aa);
            max::atomarray_clear(clone);
            max::object_free(clone);
        } else {
            throw "could not create array"; 
        }
    }

    void fill_float_vector(std::vector<float> &array, const min::atom &atom) {
        auto name_max = max::atom_getsym(&atom); 
        max::t_atomarray* aa = arrayobj_findregistered_retain(name_max);
        if (aa) {
            if (array.size() != 0) {
                throw "array not empty";
            }
            max::t_atomarray* clone = (max::t_atomarray*)object_clone((max::t_object*)aa); // CLONE, do not potentially modify upstream data
            max::t_atom atom_elt; 
            for (long i = 0; i < max::atomarray_getsize(clone); i++) {
                max::atomarray_getindex(clone, i, &atom_elt);                
                array.emplace_back(max::atom_getfloat(&atom_elt)); 
            }
            arrayobj_release(aa);
            max::atomarray_clear(clone);
            max::object_free(clone);
        } else {
            throw "could not create array"; 
        }
    }

    StaticBuffer<float> static_buffer_from_array(const min::atom &atom) {
        auto name_max = max::atom_getsym(&atom); 
        max::t_atomarray* aa = arrayobj_findregistered_retain(name_max);
        if (aa) {
            max::t_atomarray* clone = (max::t_atomarray*)object_clone((max::t_object*)aa); // CLONE, do not potentially modify upstream data
            long array_size = max::atomarray_getsize(clone);
            StaticBuffer<float> out_buffer(1, array_size);
            max::t_atom atom_elt; 
            for (long i = 0; i < max::atomarray_getsize(clone); i++) {
                max::atomarray_getindex(clone, i, &atom_elt);                
                out_buffer.put(max::atom_getfloat(&atom_elt), 0, i); 
            }
            arrayobj_release(aa);
            max::atomarray_clear(clone);
            max::object_free(clone);
            return out_buffer;
        } else {
            throw std::string("could not create array"); 
        }
    }

}