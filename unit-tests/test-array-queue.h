#ifndef APOLLON_FC_UNIT_TESTS_TEST_ARRAY_QUEUE_H
#define APOLLON_FC_UNIT_TESTS_TEST_ARRAY_QUEUE_H

#include "test-framework.h"
#include "../src/arrayQueue.h"

void unitArrayQueue() {
  int8_t __arr[8];
  ArrayQueue<int8_t> queue(__arr, 8);

  TEST_UNIT(ArrayQueue, {
    TEST_SECTION(Append, {
      queue.append(1);
      queue.append(2);
      queue.append(3);
      queue.append(4);
      TEST_CASE(1.1, queue.getTail() == 1);
      TEST_CASE(1.2, queue.getHead() == 4);
      TEST_CASE(1.3, queue.size == 4);
      queue.append(5);
      queue.append(6);
      queue.append(7);
      queue.append(8);
      TEST_CASE(1.4, queue.getTail() == 1);
      TEST_CASE(1.5, queue.getHead() == 8);
      TEST_CASE(1.6, queue.size == 8);
    }); /* Append */

    TEST_SECTION(Remove and Clear, {
      queue.remove();
      TEST_CASE(2.1, queue.size == 7);
      TEST_CASE(2.2, queue.getTail() == 2);
      TEST_CASE(2.3, queue.getHead() == 8);
      queue.remove();
      TEST_CASE(2.4, queue.size == 6);
      TEST_CASE(2.5, queue.getTail() == 3);
      TEST_CASE(2.6, queue.getHead() == 8);
      queue.clear();
      TEST_CASE(2.7, queue.size == 0);
      queue.append(16);
      TEST_CASE(2.8, queue.size == 1);
      TEST_CASE(2.9, queue.getTail() == 16);
      TEST_CASE(2.10, queue.getHead() == 16);
    }); /* Remove and Clear */

    TEST_SECTION(Overflow Append, {
      queue.append(17);
      queue.append(18);
      queue.append(19);
      queue.append(20);
      queue.append(21);
      queue.append(22);
      TEST_CASE(3.1, queue.size == 7);
      queue.append(23);
      TEST_CASE(3.2, queue.size == 8);
      TEST_CASE(3.3, queue.getTail() == 16);
      TEST_CASE(3.4, queue.getHead() == 23);
      queue.append(24);
      TEST_CASE(3.5, queue.size == 8);
      TEST_CASE(3.6, queue.getTail() == 17);
      TEST_CASE(3.7, queue.getHead() == 24);
      queue.append(25);
      queue.append(26);
      queue.append(27);
      queue.append(28);
      TEST_CASE(3.8, queue.size == 8);
      TEST_CASE(3.9, queue.getTail() == 21);
      TEST_CASE(3.10, queue.getHead() == 28);
    }); /* Overflow Append */

    TEST_SECTION(getIndex, {
      TEST_CASE(4.1, queue.getIndex(0) == 21);
      TEST_CASE(4.2, queue.getIndex(1) == 22);
      TEST_CASE(4.3, queue.getIndex(2) == 23);
      TEST_CASE(4.4, queue.getIndex(3) == 24);
      TEST_CASE(4.5, queue.getIndex(4) == 25);
      TEST_CASE(4.6, queue.getIndex(5) == 26);
      TEST_CASE(4.7, queue.getIndex(6) == 27);
      TEST_CASE(4.8, queue.getIndex(7) == 28);
      TEST_CASE(4.9, queue.getTail() == queue.getIndex(0));
      TEST_CASE(4.10, queue.getHead() == queue.getIndex(queue.size - 1));
    }); /* getIndex */
  }); /* ArrayQueue */
}

#endif /* APOLLON_FC_UNIT_TESTS_TEST_ARRAY_QUEUE_H */
