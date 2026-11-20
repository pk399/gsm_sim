#include <stdio.h>
#include <assert.h>
#include <osmocom/core/bits.h>
#include <osmocom/codec/codec.h>
#include <osmocom/coding/gsm0503_coding.h>

int main(int argc, char** argv) {
    double p = 0.002; // bit flipping probability
    if (argc > 1) {
        p = atof(argv[1]);
    }

    srand(42);
    if (argc > 2) {
        srand(atoi(argv[2]));
    }

    int r;

    uint8_t data_tx[GSM_FR_BYTES] = {};
    data_tx[0] = 0xd << 4;
    ubit_t bursts[8*116] = {};

    r = gsm0503_tch_fr_encode(bursts, data_tx, GSM_FR_BYTES, 0);
    assert(!r);

    sbit_t soft_bursts[sizeof bursts] = {};
    osmo_ubit2sbit(soft_bursts, bursts, sizeof bursts);

    // channel
    for (int i = 0; i < sizeof soft_bursts; i++) {
        double a = (double) rand() / RAND_MAX;
        if (a < p) {
            soft_bursts[i] *= -1;
        }
    }

    uint8_t data_rx[GSM_FR_BYTES] = {};
    r = gsm0503_tch_fr_decode(data_rx, soft_bursts, 0, 0, NULL, NULL);
    // somehow fails assert(r > 0);

    int n_errors = 0, n_bits_total = GSM_FR_BYTES*8;

    for (int i = 0; i < n_bits_total; i++) {
        ubit_t tx = data_tx[i >> 3];
        ubit_t rx = data_rx[i >> 3];

        if (((tx >> i%8) & 1) != ((rx >> i%8) & 1)) {
            n_errors += 1;
        }
    }
    printf("n_errors\t%d\n", n_errors);
    printf("n_bits_total\t%d\n", n_bits_total);

    return 0;
}
