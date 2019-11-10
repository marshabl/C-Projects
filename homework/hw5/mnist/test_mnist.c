#include "mnist.h"

#include <CUnit/Basic.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>


static int init_suite(void)
{
   return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
static int clean_suite(void)
{
   return 0;
}

static void test_mnist_create()
{
	mnist_dataset_handle h = mnist_create(28, 28);

	CU_ASSERT_PTR_NOT_NULL(h);

	unsigned int width, height;
	mnist_image_size (h, &width, &height);

	CU_ASSERT_EQUAL(width, 28);
	mnist_free(h);
}

static void test_mnist_open()
{
	mnist_dataset_handle h = mnist_open("train");

	int imagecount = mnist_image_count(h);

	CU_ASSERT_EQUAL(imagecount, 60000);

	unsigned int width, height;
        mnist_image_size (h, &width, &height);

        CU_ASSERT_EQUAL(width, 28);

	mnist_free(h);
}

static void test_image_funcs()
{
	mnist_dataset_handle h = mnist_open("train");
	mnist_image_handle first = mnist_image_begin(h);
	mnist_image_handle second = mnist_image_next(first);
	mnist_image_handle third = mnist_image_next(second);

	int firstlabel = mnist_image_label(first);
	int secondlabel = mnist_image_label(second);
	int thirdlabel = mnist_image_label(third);

	CU_ASSERT_EQUAL(firstlabel, 5);
	CU_ASSERT_EQUAL(secondlabel, 0);
	CU_ASSERT_EQUAL(thirdlabel, 4);
	
	mnist_image_handle mys = mnist_image_remove(h, second);
	int myslabel = mnist_image_label(mys);

	CU_ASSERT_EQUAL(myslabel, thirdlabel);

	mnist_free(h);
}




/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   srand(time(NULL));
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
   if (NULL == pSuite)
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (
       (NULL == CU_add_test(pSuite, "mnist_create", test_mnist_create)) ||
       (NULL == CU_add_test(pSuite, "mnist_open", test_mnist_open)) ||
       (NULL == CU_add_test(pSuite, "mnist_image_funcs", test_image_funcs))
    )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

