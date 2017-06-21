#include "solution.hpp"
#include <thread>

using Lst = List<int>;
using namespace std;

void doSmth1(Lst &list);
void doSmth2(Lst &list);

int main()
{
	Lst list;
	srand(time(NULL));

  thread th1(doSmth1, ref(list));
  thread th2(doSmth1, ref(list));
  thread th3(doSmth1, ref(list));

  th1.join();
  th2.join();
  th3.join();

    //Show

	for (int i(0); i < list.Size(); i++) {
		cout << list.get(i) << " ";
	}

    //
    thread th4(doSmth2, ref(list));
    thread th5(doSmth2, ref(list));

    th4.join();
    th5.join();

    return EXIT_SUCCESS;
}

void doSmth1(Lst &list) {
    for (int i = 0; i < 10; i++) {
        int a = (rand() % 100);
        list.push(a);
    }
    int s = list.Size();
    cout << s;
}

void doSmth2(Lst &list) {
    for (int i = 0; i < list.Size() / 2 - 1; i++) {
        string s = to_string(list.get(0)) + '\n';
        cout << s;
        list.erase(0);
    }
}