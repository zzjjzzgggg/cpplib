#include <cstdio>
#include "../os/osutils.h"
#include "../adv/rngutils.h"

class HyperLL {
public:
    typedef unsigned char uchar;
    typedef unsigned long long uint64;

    static constexpr uchar CLZ_TABLE_4BIT[16] = {4, 3, 2, 2, 1, 1, 1, 1,
                                                 0, 0, 0, 0, 0, 0, 0, 0};

private:
    int p_, m_;
    double alpha_;
    std::vector<uchar> regs_;

    rngutils::default_rng rng_;
    // rngutils::mt19937_rng rng_;

private:
    double getAlpha() const {
        switch (m_) {
            case 16:
                return 0.673;
            case 32:
                return 0.697;
            case 64:
                return 0.709;
            default:
                return 0.7213 / (1 + 1.079 / m_);
        }
    }

    double beta(const double ez) const {
        double zl = std::log(ez + 1);
        return -0.37331876643753059 * ez + -1.41704077448122989 * zl +
               0.40729184796612533 * std::pow(zl, 2) +
               1.56152033906584164 * std::pow(zl, 3) +
               -0.99242233534286128 * std::pow(zl, 4) +
               0.26064681399483092 * std::pow(zl, 5) +
               -0.03053811369682807 * std::pow(zl, 6) +
               0.00155770210179105 * std::pow(zl, 7);
    }

    uchar clz8(uint64 x) const {
        if ((x & 0xFF00000000000000) == 0) return 8;
        uchar n;
        if ((x & 0xF000000000000000) == 0) {
            n = 4;
            x <<= 4;
        } else
            n = 0;
        return n + CLZ_TABLE_4BIT[x >> 60];
    }

    std::tuple<double, double, double> count() const {
        double sum = 0, ez = 0;
        for (int v : regs_) {
            sum += 1.0 / (1 << v);
            if (v == 0) ez++;
        }
        sum += beta(ez);
        double est_HLL = alpha_ * m_ * (m_ - ez) / sum + 0.5;
        double est_LC = m_ * std::log(m_ / ez);
        double est = est_HLL < 10000 ? est_LC : est_HLL;
        return std::make_tuple(est_HLL, est_LC, est);
    }

    double countHLL() const {
        double sum = 0, ez = 0;
        for (int v : regs_) {
            sum += 1.0 / (1 << v);
            if (v == 0) ez++;
        }
        sum += beta(ez);
        // Use LogLog-Beta bias estimation
        return alpha_ * m_ * (m_ - ez) / sum + 0.5;
    }

    double countLC() const {
        double ez = 0;
        for (int v : regs_) {
            if (v == 0) ez++;
        }
        return m_ * std::log(m_ / ez);
    }

public:
    HyperLL(const int p) : p_(p) {
        m_ = 1 << p_;
        alpha_ = getAlpha();
        regs_.resize(m_);
        std::fill(regs_.begin(), regs_.end(), 0);
    }

    void estimate(const int n) {
        printf("real\t\thll\t\tlc\t\tall\t\ter_hll\t\ter_lc\t\ter_all\n");
        int step = 1;
        for (int i = 1; i <= n; i++) {
            uint64 x = rng_.randint<uint64>();
            int reg_idx = (x >> (64 - p_)) & ((1 << p_) - 1);
            uchar reg_rho = clz8(x << p_ | 1 << (p_ - 1)) + 1;
            if (reg_rho > regs_[reg_idx]) regs_[reg_idx] = reg_rho;

            if (i % step == 0) {
                auto ests = count();
                double est_hll = std::get<0>(ests), est_lc = std::get<1>(ests),
                       est = std::get<2>(ests),
                       err_hll = std::abs(est_hll - i) / i * 100,
                       err_lc = std::abs(est_lc - i) / i * 100,
                       err = std::abs(est - i) / i * 100;
                printf(
                    "%d\t\t%.1f\t\t%.1f\t\t%.1f\t\t%.1f%%\t\t%.1f%%\t\t%.1f%%"
                    "\n",
                    i, est_hll, est_lc, est, err_hll, err_lc, err);
                step *= 2;
            }
        }
    }

}; /* countHLL */

void test_HLL() {
    HyperLL hll(12);
    hll.estimate(1000000);
    // hll.test_clz();
}

int main(int argc, char* argv[]) {
    osutils::Timer tm;

    test_HLL();

    return 0;
}
