#ifndef Forklift_Examples_Potts_hpp
#define Forklift_Examples_Potts_hpp

#include <Tools/Misc.hpp>
#include <Tools/RNG.hpp>
#include <vector>

namespace Forklift
{

namespace Examples
{

class Potts
{
	private:
		static constexpr int num_colors = 5;
        static constexpr int n = 32;

        // State
		std::vector<std::vector<int>> pixels;
        std::vector<double> s_values;

		// So we can just update on the fly
		int score;
		int score2;

		void compute_score();
		void compute_scalars();

	public:

		Potts() = delete;
        Potts(Tools::RNG& rng);
		double perturb(Tools::RNG& rng);
        std::vector<double> scalars() const;
        double x() const;
        double y() const;
        std::vector<unsigned char> to_bytes() const;

};

/* IMPLEMENTATIONS FOLLOW */


Potts::Potts(Tools::RNG& rng)
:pixels(n, std::vector<int>(n))
,s_values(2)
{
	for(size_t i=0; i<pixels.size(); i++)
		for(size_t j=0; j<pixels[i].size(); j++)
			pixels[i][j] = rng.rand_int(num_colors);

	compute_score();
	compute_scalars();
}

void Potts::compute_score()
{
	score = 0;
	score2 = 0;

	// Coordinates of neighbouring cells
	std::vector<int> ii(4);
    std::vector<int> jj(4);

	for(size_t i=0; i<pixels.size(); i++)
	{
		for(size_t j=0; j<pixels[i].size(); j++)
		{
			for(int k=0; k<4; k++)
			{
				ii[k] = i;
				jj[k] = j;
			}

			// Down, up, right, left
			ii[0] = Tools::mod(i + 1, static_cast<int>(pixels.size()));
			ii[1] = Tools::mod(i - 1, static_cast<int>(pixels.size()));
			jj[2] = Tools::mod(j + 1, static_cast<int>(pixels[i].size()));
			jj[3] = Tools::mod(j - 1, static_cast<int>(pixels[i].size()));

			for(int k=0; k<4; k++)
			{
				if(pixels[i][j] == pixels[ii[k]][jj[k]])
				{
					score++;
					if(k >= 2)
						score2++;
				}
			}
		}
	}
}

double Potts::perturb(Tools::RNG& rng)
{
	int reps = 1;
	if(rng.rand() <= 0.5)
		reps += 1 + rng.rand_int(9);

	// Which cell is being perturbed
	int i, j;

	// Coordinates of neighbouring cells
	std::vector<int> ii(4);
    std::vector<int> jj(4);
	for(int z=0; z<reps; z++)
	{
		i = rng.rand_int(pixels.size());
		j = rng.rand_int(pixels[0].size());

		for(int k=0; k<4; k++)
		{
			ii[k] = i;
			jj[k] = j;
		}

		// Down, up, right, left
		ii[0] = Tools::mod(i + 1, static_cast<int>(pixels.size()));
		ii[1] = Tools::mod(i - 1, static_cast<int>(pixels.size()));
		jj[2] = Tools::mod(j + 1, static_cast<int>(pixels[i].size()));
		jj[3] = Tools::mod(j - 1, static_cast<int>(pixels[i].size()));

		// Calculate negative part of delta score
		for(int k=0; k<4; k++)
		{
			if(pixels[i][j] == pixels[ii[k]][jj[k]])
			{
				score--;
				if(k >= 2)
					score2--;
			}
		}

		// Perturb the cell
        auto orig = pixels[i][j];
        do
        {
    		pixels[i][j] = rng.rand_int(num_colors);
        }while(pixels[i][j] == orig);

		// Calculate positive part of delta score
		for(int k=0; k<4; k++)
		{
			if(pixels[i][j] == pixels[ii[k]][jj[k]])
			{
				++score;
				if(k >= 2)
					++score2;
			}
		}
	}

	compute_scalars();
	return 0.0;
}


void Potts::compute_scalars()
{
	s_values[0] = 0.5*score;
	s_values[1] = 0.5*score2;
}

double Potts::x() const
{
    return s_values[0];
}

double Potts::y() const
{
    return s_values[1];
}

std::vector<unsigned char> Potts::to_bytes() const
{
    int num_bytes = sizeof(int)*n*n;
    std::vector<unsigned char> result(num_bytes);
    int pos = 0;
    for(int i=0; i<n; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            std::memcpy(&(result[pos]), &(pixels[i][j]), sizeof(int));
            pos += sizeof(int);
        }
    }
    return result;
}

} // namespace Examples

} // namespace

#endif
