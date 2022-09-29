#ifndef LPFILTER1_HPP_INCLUDED
#define LPFILTER1_HPP_INCLUDED

#include <types.hpp>


class LpFilter1
{
public:

	fp32_t& Run(const fp32_t& input);
	fp32_t& RunLim(const fp32_t& input,const fp32_t& min,const fp32_t& max);
	void SetParameters(const fp32_t& f_filt,const fp32_t& t_samp);
	void InitValue(const fp32_t& output);
	bool   b_in_limit;

private:
	fp32_t m_xgain;
	fp32_t m_ygain;
	fp32_t m_y1;
	fp32_t m_y1_old;
	fp32_t m_max;
	fp32_t m_min;
};

#endif //\#ifndef LPFILTER1_HPP_INCLUDED
