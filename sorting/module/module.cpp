#include <module.h>
#include <sort_base.h>
#include <stddef.h>
#include <assert.h>
static sort_context *sc = NULL;

static module *base = NULL;
static void sc_sort(void *base, size_t nelem, size_t size, compare cmp = NULL)
{
    assert(sc != NULL);
    if ( sc->get_algorithm() == NULL)
    {
        std::cout << " no available algorithm to apply\n";
        return;
    }
    sc->sort(base, nelem, size, cmp);
} 

static void sc_test_module(size_t count = 30)
{
   assert(sc != NULL); 
   sc->int_test(count);
}
void regisgter_module(module *mod)
{

   if (base == NULL)
   {
      base = mod;
      return;
    }
    module *m = base;
    while(m->next != NULL)
        m = m->next;
   m->next = mod;
}


extern void destroy_module(const module *m)
{
    if (m != NULL)
    {
        delete m;
    }
    if (sc != NULL)
        delete sc;

}

unsigned int sc_get_runtime(void)
{
    assert(sc != NULL);
    return sc->get_runtime();
}
const char *sc_get_description(void)
{
    assert(sc != NULL);
    return sc->get_algorithm()->get_description();
}
void sc_set_verbose(int b, print_element p)
{
    assert(sc != NULL);
    sc->set_verbose(b != 0, p); 
}

extern "C" module *get_module(const char *name)
{
    module * m  = new module;
    sc = new sort_context;
    sc->select_algorithm(name);
    m->set_verbose = sc_set_verbose;
    m->sort = sc_sort;
    m->test_module = sc_test_module;
    m->get_runtime = sc_get_runtime;
    m->get_description = sc_get_description;
    return m;
}




