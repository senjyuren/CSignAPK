#include "Kernel.hpp"

int main()
{
    auto &&timeOne = GetTickCount();
    (new m::APKObjects::Builder())
            ->setAPKPath(R"(C:\Users\senjy\Desktop\app-release.apk)")
            .setPrivateKeyPath(R"(C:\Users\senjy\Desktop\platform.pk8)")
            .setCertPath(R"(C:\Users\senjy\Desktop\platform.x509.pem)")
            .build();
    auto &&timeTwo = GetTickCount();

    printf("Time Consuming: %ld\r\n", (timeTwo - timeOne));
    system("pause");
    return 0;
}
