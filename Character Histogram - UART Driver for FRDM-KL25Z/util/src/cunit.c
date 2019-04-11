
/**
 * @\file	util_test.c
 * @\author	Sorabh Gandhi / Sanju Prakash Kannioth
 * @\brief	This file contains the unit test function for all the circular buffer functionalities
 * @\date	09/25/2018
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*CUnit library headers */
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>

#include "circular_buffer.h"

#define MAX 255

c_buff *cb = NULL;
//malloc(sizeof(c_buff));

size_t length;
uint8_t *array = NULL;

int init_test (void)
{
	cb = (c_buff *)malloc(sizeof(c_buff));

	srandom(time(NULL));
	length = (random() % MAX);

	array = (uint8_t *)malloc(sizeof(uint8_t) * length);

	return 0;
}


int deinit_test (void)
{
	free(cb);
	
	return 0;
}

void test_cb_new()
{
	CU_ASSERT_EQUAL(cb_new(cb, length), 1);
}

void test_cb_add_data()
{
	int i = 0;

	for (i = 0; i < (length); i++)
	{
		array[i] = (uint8_t)(random() % MAX);
		CU_ASSERT_EQUAL(cb_add_data(cb, array[i]), 1);
	}

	CU_ASSERT_EQUAL(cb_add_data(cb, 10), -5);	//-5
	CU_ASSERT_EQUAL(cb_add_data(cb, 20), -5);   //-5
	CU_ASSERT_EQUAL(cb_add_data(cb, 30), -5);   //-5
}

void test_cb_getSize()
{
	CU_ASSERT_EQUAL(cb_getSize(cb), length);
}

void test_cb_remove_data()
{
	uint8_t data = 0;
	int i = 0;

	for (i = 0; i < (length); i++)
	{
		CU_ASSERT_EQUAL(cb_remove_data(cb, &data), 1);
		//printf("data = %d\n", data);
		CU_ASSERT_EQUAL(data, array[i]);
	}
	
	CU_ASSERT_EQUAL(cb_remove_data(cb, &data), -4);		//-4*/
	CU_ASSERT_EQUAL(cb_remove_data(cb, &data), -4);     //-4*/
	CU_ASSERT_EQUAL(cb_remove_data(cb, &data), -4);     //-4*/
}

void test_cb_resize()
{
	int i = 0;

    for (i = 0; i < (length); i++)
    {
        array[i] = (uint8_t)(random() % MAX);
        CU_ASSERT_EQUAL(cb_add_data(cb, array[i]), 1);
    }

	CU_ASSERT_EQUAL(cb_resize(cb, length), 1);

	for (i = 0; i < (length/2); i++)
    {
        CU_ASSERT_EQUAL(cb_add_data(cb, i), 1);
    }

	CU_ASSERT_EQUAL(cb_getSize(cb), (length + (length/2)));

	uint8_t data = 0;
	for (i = 0; i < (length/2); i++)
    {
        CU_ASSERT_EQUAL(cb_remove_data(cb, &data), 1);
    }
}

void test_cb_delete()
{
	CU_ASSERT_EQUAL(cb_delete(cb), 1);
}

int register_test_suite(void) {

    CU_pSuite pSuite = NULL;

   	pSuite = CU_add_suite("Functionality Test", init_test, deinit_test);
    if (NULL == pSuite) {
        return -1;
    }
    /*Adding tests to the suite */
    if ((NULL == CU_add_test(pSuite, "Init functionality test", test_cb_new)) ||
    	(NULL == CU_add_test(pSuite, "Add functionality test", test_cb_add_data)) ||
		(NULL == CU_add_test(pSuite, "check cb size functionality test", test_cb_getSize)) ||
    	(NULL == CU_add_test(pSuite, "remove functionality test", test_cb_remove_data)) ||
		(NULL == CU_add_test(pSuite, "resize functionality test", test_cb_resize)) ||
    	(NULL == CU_add_test(pSuite, "delete functionality test", test_cb_delete))) 
    {
        return -1;
    }
    return 0;
}

int main()
{
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

 	//register_test_suite will add a test suite to the CUnit test registry
    if (register_test_suite() == -1) {
        CU_cleanup_registry();
        return CU_get_error();
    }

	/* Run all tests using CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();

    /* Run all tests using the CUnit Automated interface */
	//CU_set_output_filename("test");
	//CU_automated_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}
