#ifndef CreditCard_H
#define CreditCard_H

typedef struct {
    char cardNumber[16];
    float creditLimit;
    float balance;
    char expiryDate[11];
} CreditCard;

#endif


