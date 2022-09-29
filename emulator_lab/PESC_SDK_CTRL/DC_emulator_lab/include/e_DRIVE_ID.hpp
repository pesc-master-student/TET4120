/*
 * e_DRIVE_ID.hpp
 *
 *  Created on: 20. jul. 2022
 *      Author: lokaladm_thohau
 */

#ifndef SRC_E_DRIVE_ID_HPP_
#define SRC_E_DRIVE_ID_HPP_

enum e_DRIVE_ID {
		NoN_INITIALIZED = 0,
		DC_DRIVE = 1,
		IM = 2,
		PM = 4,
		SM = 8,
		PM6 = 16,
		IM6 = 32,
		FIRMWARE_SOFTWARE_MISMATCH = 2048,
};

#endif /* SRC_E_DRIVE_ID_HPP_ */
