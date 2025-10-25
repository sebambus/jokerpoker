#include "voucher.h"

#include "readcsv.h"

const char* voucherName(voucher v) {
    return readcsv("voucher.csv", v, 0);
}