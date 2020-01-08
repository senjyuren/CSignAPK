#include "Kernel.hpp"

constexpr static Jchar TEST_PUB_KEY[] =
                               "-----BEGIN RSA PUBLIC KEY-----\n"
                               "MIIBCgKCAQEAqeA2foi9PTtcS6mEznVZ5mmqAk37VkpkM2vBdjRZpa/60E50YSKg\n"
                               "bKUIk2i6VHLKg3snjLZ8MAAjU0H0rKfE0gNWlsfcfLF/9anaiKsUxgzfNgs2JS9F\n"
                               "H9sf7hZzNqF1wd5fSCX4EJSUuHdTEC4zGObXhFkZXa81AauLqNDmHn2C18nzqPWl\n"
                               "xFz3dPB77POikTatS/oJgDqL/MCzH7YinicySQ0Ypf/LjC+xPlCRhkDVWmajR3zg\n"
                               "PZtGGjPQsWgCFJq61j44hHSybZhczzocr3amzQPYJJEAJuSKIjexRgW+/qgaRhXT\n"
                               "te+yRkLZngFxhLzS7TJFcyWms5e28rocgwIDAQAB\n"
                               "-----END RSA PUBLIC KEY-----";

constexpr static Jchar TEST_PRI_KEY[] =
                               "-----BEGIN RSA PRIVATE KEY-----\n"
                               "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCp4DZ+iL09O1xL\n"
                               "qYTOdVnmaaoCTftWSmQza8F2NFmlr/rQTnRhIqBspQiTaLpUcsqDeyeMtnwwACNT\n"
                               "QfSsp8TSA1aWx9x8sX/1qdqIqxTGDN82CzYlL0Uf2x/uFnM2oXXB3l9IJfgQlJS4\n"
                               "d1MQLjMY5teEWRldrzUBq4uo0OYefYLXyfOo9aXEXPd08Hvs86KRNq1L+gmAOov8\n"
                               "wLMftiKeJzJJDRil/8uML7E+UJGGQNVaZqNHfOA9m0YaM9CxaAIUmrrWPjiEdLJt\n"
                               "mFzPOhyvdqbNA9gkkQAm5IoiN7FGBb7+qBpGFdO177JGQtmeAXGEvNLtMkVzJaaz\n"
                               "l7byuhyDAgMBAAECggEBAKGgvdiom30uBIPM15pkV1P+Rw6ISdHxV+MA7IJeDsmx\n"
                               "ZW9apiphesfEQiOW6mo9CegNfjTIz+IPz4yA3Z6ExaSYHoO5x1QwwjHoGRAdztcd\n"
                               "GidehXbdweQsZfXvEFHzT4pU3M1Vqu+DnvjgN3zWqDFSOc4nxBW6BTIAmRHXKIvq\n"
                               "i9Vj7nBjbkXuD8xG2pQuNhoEXY/x35QbJmknTInHqqhtx+mS4xh2TU4V404qt+0O\n"
                               "Oj03oszRQrlYqrOgxVWZuMoYk4LvKYJ3S9aUd6weFwHfn1xzZ7/RO58ICYxwCbGT\n"
                               "Rf+ME1Fvkg++AlQfNrinWhoZX9tdXTgSJq/ko6WBTjECgYEA3S17x8HEbmuPTdN1\n"
                               "tHxB2cynrVsi0iS36dOMeOIgJ0qpfgw1SBaqR4RBJS1WlI4pq4JeBOB5WHJWA2en\n"
                               "WDU/nEFsWyYRwfWsEWxLJCPUtYfKOFfZcfsK5EA48ZO252KRRnHSjIGLT711aoFr\n"
                               "7Hu8tH1MCzLiwbhyz2jkC3UeJ4sCgYEAxJ8FLhkmGIE0V8knGMQzowEWqhnoDhF7\n"
                               "ByOiTlyGNet3sTGrKhNIswJIJgs2VshRZiw42bNjfJVdCAUkR+HcEPTNzAoCDRzs\n"
                               "ADmbj9w5XdgC8CJtjWTIeAlBsyJFVPXr99J3uxQMk5UoESHNhnnLPIQde75pPt+t\n"
                               "/T9IdLwUPekCgYBPcogU7/hURZSbOwr7wuqQhp/weF0gqQnbFx9NSMSN+Ja7bT0y\n"
                               "YNnXiJ0x8oA4KnyzW8fT7qIH8EIPRQtqdu4bfbCjaSBV0J53P7QaDjjY4Cb2aC/2\n"
                               "bluyM5OAyzOXF+PhLv+Xcu0MvpxPnp+aubPGDX/kQeoL9epfsGjB6NblEwKBgAa7\n"
                               "ey3s5oUIR4aJ+LC8Zxg5vVDtuihhKNlBuxj7ygyml16V2MqEBoU6msXStHWcPk1F\n"
                               "Tl0F624knx7bLjbX4PUjrLhDz3Yre1k2n8D5RW2VK+SVCTNCNCsIBhvgHIegNAg0\n"
                               "SK07LoMPJGQRyDvGh9L2oxvNlUT127tEI3L7zaWhAoGAIfFclp2i/7TXr/R07kQ4\n"
                               "ucYqz2yq61+354mkppkW56JsYCqom8S1/VVMwPwCuAxJGhGy/S03kW6N63u3qMAo\n"
                               "iVLUM8fvoPwCrcgM8yLHl3Vt36xLXxS3+ckibelKZHjMR1VvA3KWGWjGgy4555fH\n"
                               "ce5GWWG5ljMNle7zFhkJqY4=\n"
                               "-----END RSA PRIVATE KEY-----";

constexpr Jchar TEST_CA[] =
        "-----BEGIN CERTIFICATE-----\n"
        "MIID6TCCAtGgAwIBAgIJAPEIaXR0Ft7HMA0GCSqGSIb3DQEBBQUAMIGKMQswCQYD\n"
        "VQQGEwJDTjERMA8GA1UECAwIU2hhbmdoYWkxDzANBgNVBAcMBlB1RG9uZzETMBEG\n"
        "A1UECgwKWGNoZW5ndGVjaDELMAkGA1UECwwCU1cxDzANBgNVBAMMBnhjaGVuZzEk\n"
        "MCIGCSqGSIb3DQEJARYVeGNoZW5nQHhjaGVuZ3RlY2guY29tMB4XDTE3MTIxMTEw\n"
        "MDE1MloXDTQ1MDQyODEwMDE1MlowgYoxCzAJBgNVBAYTAkNOMREwDwYDVQQIDAhT\n"
        "aGFuZ2hhaTEPMA0GA1UEBwwGUHVEb25nMRMwEQYDVQQKDApYY2hlbmd0ZWNoMQsw\n"
        "CQYDVQQLDAJTVzEPMA0GA1UEAwwGeGNoZW5nMSQwIgYJKoZIhvcNAQkBFhV4Y2hl\n"
        "bmdAeGNoZW5ndGVjaC5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\n"
        "AQCp4DZ+iL09O1xLqYTOdVnmaaoCTftWSmQza8F2NFmlr/rQTnRhIqBspQiTaLpU\n"
        "csqDeyeMtnwwACNTQfSsp8TSA1aWx9x8sX/1qdqIqxTGDN82CzYlL0Uf2x/uFnM2\n"
        "oXXB3l9IJfgQlJS4d1MQLjMY5teEWRldrzUBq4uo0OYefYLXyfOo9aXEXPd08Hvs\n"
        "86KRNq1L+gmAOov8wLMftiKeJzJJDRil/8uML7E+UJGGQNVaZqNHfOA9m0YaM9Cx\n"
        "aAIUmrrWPjiEdLJtmFzPOhyvdqbNA9gkkQAm5IoiN7FGBb7+qBpGFdO177JGQtme\n"
        "AXGEvNLtMkVzJaazl7byuhyDAgMBAAGjUDBOMB0GA1UdDgQWBBRqp1ZaRt/L9EbU\n"
        "6DIi4Hpp3geRJDAfBgNVHSMEGDAWgBRqp1ZaRt/L9EbU6DIi4Hpp3geRJDAMBgNV\n"
        "HRMEBTADAQH/MA0GCSqGSIb3DQEBBQUAA4IBAQBPW+snm5zaBxuf+NC2Y3CNwHxz\n"
        "ARnjKLXKJWuwSD1AvOwEtKi4ysfex3Wo3QgEWuelTPHcsBb5LmbIWs8IenSFG570\n"
        "IIMjc30+YJ9srBA6CY5cfPZyYk/PHnO8J6xiTARsK7YdrqdjHG9g3tjwHtzprQ81\n"
        "viKvJ+7AC48QQ3i1+UqVP1UmIadTvkF54txP8BKFqhRpqlx2zM2T9FRiRVQZZ2nX\n"
        "ToKNjQAhjSSgs+5bayjanztWpy7ChkHOIVtP1PhEWltsEejjck19y/lLdrFJp5qH\n"
        "jVPT6hlmsj/gutE35oa0MdW1gxKdfEfXC3NOFzvqrEWV3Awyz82rBKNfzxZG\n"
        "-----END CERTIFICATE-----";

int main()
{
    auto &&x509 = (new m::UtilsX509Adapter::Builder())
            ->setCity("PuDong")
            .setCommon("xcheng")
            .setEmail("xcheng@xchengtech.com")
            .setCountry("CN")
            .setOrganization("Xchengtech")
            .setOrganizationUnit("SW")
            .setProvince("Shanghai")
            .setVersion(m::UtilsX509Version::V3)
            .setExpDate(60)
            .setPublicKey(TEST_PUB_KEY)
            .setPrivateKey(TEST_PRI_KEY)
            .setCAPrivateKey(TEST_PRI_KEY)
            .setCA(TEST_CA)
            .build();

    (new m::APKObjects::Builder())
//            ->setAPKPath(R"(C:\Users\senjy\Desktop\app-release.apk)")
            ->setAPKPath(R"(C:\Users\senjy\Downloads\Xyq-1.12.0.apk)")
            .setPrivateKey(TEST_PRI_KEY)
            .setCert(x509->getCert().c_str())
            .build();
    return 0;
}
