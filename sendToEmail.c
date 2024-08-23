#include "sendToEmail.h"
#include <stdio.h>

void sendToEmail(BreachType breachType) {
  const char* recipient = "a.b@c.com";
  const char* message = generateEmailMessage(breachType);
  
  if (message != NULL) {
    printf("To: %s\n", recipient);
    printf("%s\n", message);
  }
}
