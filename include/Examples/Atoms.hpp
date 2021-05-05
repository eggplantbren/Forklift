#ifndef Forklift_Examples_Atoms_hpp
#define Forklift_Examples_Atoms_hpp

#include <cassert>
#include <sstream>
#include <Tools/Misc.hpp>
#include <Tools/RNG.hpp>
#include <vector>

namespace Forklift
{

namespace Examples
{

class Atoms
{
    private:
        static constexpr int num_atoms = 20;
        static constexpr double sigma = 0.1;
        static constexpr double sigmasq = sigma*sigma;

        std::vector<double> xs, ys;
        std::vector<std::vector<double>> terms;
        double term(int i, int j) const;

    public:
        Atoms() = delete;
        Atoms(Tools::RNG& rng);
        Atoms(std::vector<unsigned char> bytes);
        double perturb(Tools::RNG& rng);
        double x() const;
        double y() const;
        std::vector<unsigned char> to_bytes() const;
};

/* IMPLEMENTATION FOLLOWS */

Atoms::Atoms(Tools::RNG& rng)
:xs(num_atoms)
,ys(num_atoms)
,terms(num_atoms)
{
    for(int i=0; i<num_atoms; ++i)
    {
        for(int j=0; j<num_atoms; ++j)
        {
            xs[i] = rng.rand();
            ys[i] = rng.rand();
        }
    }

    for(int i=0; i<num_atoms; ++i)
    {
        terms[i].reserve(i);
        for(int j=0; j<i; ++j)
            terms[i].push_back(term(i, j));
    }
}

double Atoms::term(int i, int j) const
{
    assert(j < i);
    double rsq = pow(xs[i] - xs[j], 2) + pow(ys[i] - ys[j], 2);
    return -(pow(rsq/sigmasq, -6) - pow(rsq/sigmasq, -3));
}

Atoms::Atoms(std::vector<unsigned char> bytes)
:xs(num_atoms)
,ys(num_atoms)
,terms(num_atoms)
{
     std::cerr << "NOT IMPLEMENTED." << std::endl;
}

double Atoms::perturb(Tools::RNG& rng)
{
    int k = rng.rand_int(num_atoms);

    xs[k] += rng.randh();
    ys[k] += rng.randh();
    Tools::wrap(xs[k]);
    Tools::wrap(ys[k]);

    // Row k
    for(int j=0; j<k; ++j)
        terms[k][j] = term(k, j);

    // Column k
    for(int i=k+1; i<num_atoms; ++i)
        terms[i][k] = term(i, k);

    return 0.0;
}

double Atoms::y() const
{
    double tot = 0.0;
    for(const auto& vec: terms)
        for(const auto& value: vec)
            tot += value;
    return tot;
}

double Atoms::x() const
{
    double tot = 0.0;
    double rsq;
    for(int i=0; i<num_atoms; ++i)
    {
        rsq = pow(xs[i] - 0.5, 2) + pow(ys[i] - 0.5, 2);
        tot += -0.5*rsq;
    }
    return tot;
}

std::vector<unsigned char> Atoms::to_bytes() const
{
    std::stringstream ss;
    for(double x: xs)
        ss.write((const char*)(&x), sizeof(double));
    for(double y: ys)
        ss.write((const char*)(&y), sizeof(double));
    std::string s = ss.str();
    std::vector<unsigned char> result(s.size());
    std::memcpy(&result[0], &s[0], s.size());
    return result;
}

} // namespace Examples

} // namespace Forklift

#endif

