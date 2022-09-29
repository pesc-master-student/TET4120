#ifndef RAMPFILTER_HPP_INCLUDED
#define RAMPFILTER_HPP_INCLUDED

#include <types.hpp>


class RampFilter
{
public:

	fp32_t& Run(const fp32_t& input,const fp32_t& ActualValue,const bool& Accelerate);
	void SetParameters(const fp32_t& T_increase,const fp32_t& T_decrease,const fp32_t& max,const fp32_t& min,const fp32_t& error_max,const fp32_t& deviation_max, const fp32_t& t_samp);
	void SetValues(const fp32_t& output);
	bool   b_in_limit;

private:
	fp32_t m_max;
	fp32_t m_min;
	fp32_t m_error_max;
	fp32_t m_deviation_max;
	fp32_t m_slope_increase;
	fp32_t m_slope_decrease;
	fp32_t m_tsamp_tfilter;

	fp32_t m_input_used;
	fp32_t m_output_used;
	fp32_t m_output_old;
	fp32_t m_error;
	fp32_t m_integral_input;


	bool   b_error_max;
	bool   b_deviation_max;
	bool   b_input_reached;
};

#endif //\#ifndef RAMPFILTER_HPP_INCLUDED
