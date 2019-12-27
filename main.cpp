#include "Kernel.hpp"

int main()
{
    auto &&timeOne = GetTickCount();
    (new m::APKObjects::Builder())
            ->setAPKPath(R"(C:\Users\senjy\Desktop\a.epub)")
            .build();
    auto &&timeTwo = GetTickCount();

    printf("Time Consuming: %ld\r\n", (timeTwo - timeOne));
    system("pause");
    return 0;
}
