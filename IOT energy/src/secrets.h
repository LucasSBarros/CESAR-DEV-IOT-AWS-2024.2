#include <pgmspace.h>

#define SECRET
#define THINGNAME "EnergyEsp32"

const char WIFI_SSID[] = "uaifai-brum";
const char WIFI_PASSWORD[] = "bemvindoaocesar";
const char AWS_IOT_ENDPOINT[] = "amaov1om3n8mn-ats.iot.us-east-2.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVANoZJYnNqt28+fAc83u3yJR9/XtwMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDA5MTIxNDIx
MjJaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDCR6plbITOAhCBsytP
tbeu/4qJLcjL6ze6eUmrU+2q+YIV5FiVsUs8/DJLxjQ1+C2KrTtPL8AtgL5t2HYX
riR2MHGBUMokliRGOYgpfjLJkofSySGxQ91CpTyNIaw09SsTNlSfveJGFwZ1MRnQ
yWzkXr+zlIFZ5w5ZgmHz07k0HMvC6OdVG/cMQYCfSl3KymIBpWLHIlbtGlcNTE4U
JSTj4Cl66AWlrUjsR+vArdRqT9kuDS6QeaF/UHeH/zixlEkV44i6n/zMiEsa1d5O
0qDGOOEDahJ9UgwutcCL629YZTbEGpuFm/rBIehJJuAi9gii+2CrGYPgtz3w1xnH
5YaDAgMBAAGjYDBeMB8GA1UdIwQYMBaAFC1+v0AM1Vf149cLBb0jPfHV+r6gMB0G
A1UdDgQWBBTTpMC2rCwYBcD7YqqY1gLP861NJDAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAt+XjJZkZDLaAcOPiMxpZ7i/j
dOBsbm0CjYCU2TRDf1NOjhQoQ5EKqYfTTpDhGciNW6WjYhMQp0EmMXztqP4fkTKe
dPEKa2yn5i6Glvb0wmGZ2R8GIskGDhPHzthzdl8YuLmYpU4CUbKZjEM27zZTeIYv
dO1df5cIxLL8U3R2u8OGTBpJvY3E2CiPyzgkH3wrZMrKxdFXYI7DkLh4lSIWRR0J
pz8wg+hY8NndXx6RONTA3xyhjEt25rObPtQuyKp/fkPfAcqn0f/qIzSXLu+KWFVn
l5jH5SMZ+UJpCUpgAMhdKM/TRNmnMbJ9SPIq7NMauO1fWbCLrgmPA30b9NP5dA==
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAwkeqZWyEzgIQgbMrT7W3rv+KiS3Iy+s3unlJq1PtqvmCFeRY
lbFLPPwyS8Y0Nfgtiq07Ty/ALYC+bdh2F64kdjBxgVDKJJYkRjmIKX4yyZKH0skh
sUPdQqU8jSGsNPUrEzZUn73iRhcGdTEZ0Mls5F6/s5SBWecOWYJh89O5NBzLwujn
VRv3DEGAn0pdyspiAaVixyJW7RpXDUxOFCUk4+ApeugFpa1I7EfrwK3Uak/ZLg0u
kHmhf1B3h/84sZRJFeOIup/8zIhLGtXeTtKgxjjhA2oSfVIMLrXAi+tvWGU2xBqb
hZv6wSHoSSbgIvYIovtgqxmD4Lc98NcZx+WGgwIDAQABAoIBACi79/kuCKZc/nMD
Swh6dFnn0Hc0t04yMkwR1CVtAn7/s4u9helU9A6EXlNO6nGKaJoT+kR/IY8o2ESV
xyTt6U0ymQnhE5EaPd7Pf0Ib6LAk3f+PFlZhgUnQ9WIt9jpgUTjFAnc/y+qgN0Vp
Fx2UrhUv3IfdkkV0Uajo0nraIJHRPf7PfKx9P7fBhPGlqnkkozmMmQ9nahCmGQrz
1WRjfN5FXnaZvbKJFnyi1O6qw0mbatLGxNZlU6kSr8Ld7WBanT3N0mtk7YQxW/aT
bvs/EwHoBkVTD0GpT54CT1f+C9+v/t7u9yuT3/FowwTBp9UNwCUwg4K7x0BeMkxh
jxdFZ8ECgYEA8gGcU1bvulwkkJW16gV0CRnvZoMGdnS641FCiKxwpHwzB98xMNqq
F0VxBtP7PpcmgIjuMnWO8d1EiflRsGm6O4xX+gtshp0vfh91Mq5VNath4t9+mwAH
WJZD1e87/57ESvXRXF5A1n92nspcgGanTXAi/zkSdSMwJ4N0kopNXWECgYEAzYOR
VXN9XlWVaEGSSfL/j2U+44GZUzapFcfVYEZ4gcnCxB8hCZYbMIB0dZlLip8AgB6B
ao/Dbknv5G/QKWQTs+rZtSbuljoZUvkKmzgyLycUUhUQsrOzV9qCeOe9ZKBZgB+h
H86sshpYiqeY3LHmo758e4q+pD8TMbqJMjb4qmMCgYEA7KMci/VfVlgRPPvXNKz+
aIPertuB70R3Fh34NBKB1qTHdXQN8ppuZXD2c1nGu84qzNaYK+j9zIyzH0MWWK0Z
eG7j+9E93mdG8EmsXNFQ6rjZb3FgqAmgJURRsphKj8pcYHv7qxlSxys9/0kaqEc4
YqU641uYK8grpweIbl4kBiECgYAMm/btzEEz6YJaPDOyj9M4RBkAsnHq1h63tGQW
HRzFsdBeYh6PzQLsDeWXVYl5/tl0Ausj06BFAwpR9L5L29UNo+opoDLhSr1FBvys
m10Q4Y/wnVRB8p6dVE3PB0DqjKARKV+kaPTVlKI8F55cZBnnkAdpORELWMdftLc4
OoE1SwKBgGuaAMHWsi0ofXeH3oRPiyvuH/fAsxEQn1PFbilAXPdDYxy8k2+jOxX/
ZDBGwIeGF1jCokpNW6kkO+ULWCTxQH8a1YDGSqYvxfR2mE7l1SvUNdDQNu5gpAG0
Kyq+fJ1F68FnO9d+jEOIVAz3+NlZCqHM22/ul3eHw/o/Cs2LoHA3
-----END RSA PRIVATE KEY-----
)KEY";