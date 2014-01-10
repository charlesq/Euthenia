#include <module.h>
int main(int argc, char *argv[])
{
    do 
    {
        module *msm = get_module("merge sort");
        if (msm == NULL)
            break;
        printf("%s\n",  msm->get_description());
        msm->test_module(200);
        destroy_module(msm);
    }while(0);
    do 
    {
       module * qsm = get_module("quick sort");
       if (qsm == NULL)
            break;
       printf("%s\n", qsm->get_description());
        qsm->set_verbose(0, NULL);
        qsm->test_module(200);
        destroy_module(qsm);
    } while(0);
    do
    {
        module *hsm = get_module("heap sort");
        if (hsm == NULL)
            break;
        printf("\n%s\n", hsm->get_description());
        hsm->set_verbose(0, NULL);
        hsm->test_module(200);
        destroy_module(hsm);
    }while(0);
    do
    {
        module *csm = get_module("counting sort");
        if (csm == NULL)
            break;
        csm->set_verbose(1, NULL);
        printf("\n%s\n", csm->get_description());
        csm->test_module(200);
        destroy_module(csm);
    }while(0);
    do
    {
        module *rsm = get_module("radix sort");
        if (rsm == NULL)
            break;
        printf("\n%s\n", rsm->get_description());
        rsm->set_verbose(1, NULL);
        rsm->test_module(200);
        destroy_module(rsm);
    }while(0); 
   
    return 0;
}
