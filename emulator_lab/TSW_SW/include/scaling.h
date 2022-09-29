/**********************************************************************************************
--                      ®.®®®®®.®       -- W ƒ R T S I L ƒ   N O R W A Y   A S
--                      ®       ®-®     --
--                               ®l®    -- File Name: pdb_scaling.h
--                 ®.-rrtrl--    ®r®    --
--            .ltiw‰it-®®®®®.ll® .l®    -- Date: 2014-11-17
--         .rswwiis.          ..®r.     --
--       .‰wwiiiir®            .r-      -- Author: Frank Kvamtr¯
--     ®twwwiiiis®           ®lr.       --
--    ®‰‰‰wwiiiii.        ®-rtr®        -- Project: MV Drive
--    -w‰‰wwwiiiiw‰l---lt‰sst-          --
--    -w‰‰wwwwiiiiw‰‰‰‰wwst.            -- Dependencies:
--     -i‰‰wwwwwwwwwwisr-®              --
--       .lr‰‰‰‰‰tr-.®                  --
-------------------------------------------------------------------------------
-- Description:
-- Generic scaling structure used throughout the system.
**********************************************************************************************/

#ifndef SCALING_H
#define SCALING_H


struct ScalingStruct
{
	ScalingStruct( double scale_, double offset_after_scale_ )
	:	scale( scale_ ),
		offset_after_scale( offset_after_scale_ )
	{

	}

	double scale;
	double offset_after_scale;
};

#endif // SCALING_H
