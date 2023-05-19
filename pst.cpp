/*
  Fire is a freeware UCI chess playing engine authored by Norman Schmidt.

  Fire utilizes many state-of-the-art chess programming ideas and techniques
  which have been documented in detail at https://www.chessprogramming.org/
  and demonstrated via the very strong open-source chess engine Stockfish...
  https://github.com/official-stockfish/Stockfish.

  Fire is free software: you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or any later version.

  You should have received a copy of the GNU General Public License with
  this program: copying.txt.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fire.h"
#include "position.h"

// piece sq tables assign an value to each piece depending on which sq it's located
namespace pst
{
	int psq[num_pieces][num_squares] =
	{
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0
		},
		// white king
		{
			-19400434, -12846450, -36963467, -53216318, -53216318, -36963467, -12846450, -19400434,
			-18416946, -11862962, -35193548, -50528895, -50528895, -35193548, -11862962, -18416946,
			-29230062, -22545006, -44171656, -57934139, -57934139, -44171656, -22545006, -29230062,
			-33882920, -27394472, -48103617, -61079668, -61079668, -48103617, -27394472, -33882920,
			-40239838, -33554782, -53477495, -65536043, -65536043, -53477495, -33554782, -40239838,
			-47186705, -40698257, -59703466, -70975581, -70975581, -59703466, -40698257, -47186705,
			-54985666, -48300610, -66715996, -77070607, -77070607, -66715996, -48300610, -54985666,
			-65144046, -58590062, -75891336, -85459515, -85459515, -75891336, -58590062, -65144046,
		},
		// white pawn
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			-5636096, -1638404, 196597, 2097136, 2097136, 196597, -1638404, -5636096,
			-5439487, -1245185, 786424, 2883572, 2883572, 786424, -1245185, -5439487,
			-4980728, -393215, 2097151, 4587512, 4587512, 2097151, -393215, -4980728,
			-4587508, 393224, 3342337, 6291455, 6291455, 3342337, 393224, -4587508,
			-4194285, 1245196, 4587528, 7929857, 7929857, 4587528, 1245196, -4194285,
			-4194285, 1245196, 4587528, 7929857, 7929857, 4587528, 1245196, -4194285,
			0, 0, 0, 0, 0, 0, 0, 0,
		},
		// white knight
		{
			-40435818, -31588424, -25755698, -22806567, -22806567, -25755698, -31588424, -40435818,
			-26214472, -17367079, -11534352, -8585221, -8585221, -11534352, -17367079, -26214472,
			-17563709, -6422540, -589813, 2293782, 2293782, -589813, -6422540, -17563709,
			-12320824, -1179655, 6881312, 9699372, 9699372, 6881312, -1179655, -12320824,
			-10420282, 589816, 8650783, 14024762, 14024762, 8650783, 589816, -10420282,
			-11862081, -786448, 7208983, 12582962, 12582962, 7208983, -786448, -11862081,
			-21364848, -10158142, -2097176, 786420, 786420, 131064, -10158142, -21364848,
			-135660149, -18415699, -12451900, -9568305, -9568305, -12451900, -18415699, -135660149,
		},
		// white bishop
		{
			-2097168, -4325408, -5767216, -6422576, -6422576, -5767216, -4325408, -2097168,
			-3538976, 1441808, 1441808, -524288, -524288, 1441808, 1441808, -3538976,
			-4325408, 2097168, 3538976, 2883600, 2883600, 3538976, 2097168, -4325408,
			-4325408, 524288, 3538976, 7340080, 7340080, 3538976, 524288, -4325408,
			-4980768, 0, 2883600, 6422576, 6422576, 2883600, 0, -4980768,
			-4980768, 2883600, 4325408, 3538976, 3538976, 4325408, 2883600, -4980768,
			-4325408, 2097168, 3538976, 1441808, 1441808, 3538976, 2097168, -4325408,
			-2883600, -3538976, -3538976, -2883600, -2883600, -3538976, -3538976, -2883600,
		},
		// white rook
		{
			-3735563, 1114101, 5308405, 9568245, 9568245, 5308405, 1114101, -3735563,
			-4784119, 65545, 4259849, 8519689, 8519689, 4259849, 65545, -4784119,
			-5570540, -786412, 3407892, 7733268, 7733268, 3407892, -786412, -5570540,
			-6160362, -1376234, 2818070, 7077910, 7077910, 2818070, -1376234, -6160362,
			-6619097, -1769433, 2424871, 6684711, 6684711, 2424871, -1769433, -6619097,
			-5373904, -786384, 3538992, 7929904, 7929904, 3538992, -786384, -5373904,
			-4063211, 393237, 4980757, 9240597, 7929877, 4980757, 393237, -4063211,
			-3866691, 589757, 5177277, 9437117, 9437117, 5177277, 589757, -3866691,
		},
		// white queen
		{
			-4325461, -4325500, -4325525, -4325538, -4325538, -4325525, -4325500, -4325461,
			-3932205, -1245193, -1245219, -1245232, -1245232, -1245219, -1245193, -3932205,
			-3473424, -786413, 1310774, 1310761, 1310761, 1310774, -786413, -3473424,
			-3080192, -393181, 1704005, 3539051, 3539051, 1704005, -393181, -3080192,
			-2686973, 38, 2097224, 3932270, 3932270, 2097224, 38, -2686973,
			-2293766, 393244, 2490431, 2490419, 2490419, 2490431, 393244, -2293766,
			-1900573, 786438, 786413, 786400, 786400, 786413, 786438, -1900573,
			-1441856, -1441893, -1441919, -1441932, -1441932, -1441919, -1441893, -1441856,
		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0
		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0
		},
		// black king	
		{
			65144046, 58590062, 75891336, 85459515, 85459515, 75891336, 58590062, 65144046,
			54985666, 48300610, 66715996, 77070607, 77070607, 66715996, 48300610, 54985666,
			47186705, 40698257, 59703466, 70975581, 70975581, 59703466, 40698257, 47186705,
			40239838, 33554782, 53477495, 65536043, 65536043, 53477495, 33554782, 40239838,
			33882920, 27394472, 48103617, 61079668, 61079668, 48103617, 27394472, 33882920,
			29230062, 22545006, 44171656, 57934139, 57934139, 44171656, 22545006, 29230062,
			18416946, 11862962, 35193548, 50528895, 50528895, 35193548, 11862962, 18416946,
			19400434, 12846450, 36963467, 53216318, 53216318, 36963467, 12846450, 19400434,
		},
		// black pawn	
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			4194285, -1245196, -4587528, -7929857, -7929857, -4587528, -1245196, 4194285,
			4194285, -1245196, -4587528, -7929857, -7929857, -4587528, -1245196, 4194285,
			4587508, -393224, -3342337, -6291455, -6291455, -3342337, -393224, 4587508,
			4980728, 393215, -2097151, -4587512, -4587512, -2097151, 393215, 4980728,
			5439487, 1245185, -786424, -2883572, -2883572, -786424, 1245185, 5439487,
			5636096, 1638404, -196597, -2097136, -2097136, -196597, 1638404, 5636096,
			0, 0, 0, 0, 0, 0, 0, 0,
		},
		// black knight		
		{
			135660149, 18415699, 12451900, 9568305, 9568305, 12451900, 18415699, 135660149,
			21364848, 10158142, 2097176, -786420, -786420, -131064, 10158142, 21364848,
			11862081, 786448, -7208983, -12582962, -12582962, -7208983, 786448, 11862081,
			10420282, -589816, -8650783, -14024762, -14024762, -8650783, -589816, 10420282,
			12320824, 1179655, -6881312, -9699372, -9699372, -6881312, 1179655, 12320824,
			17563709, 6422540, 589813, -2293782, -2293782, 589813, 6422540, 17563709,
			26214472, 17367079, 11534352, 8585221, 8585221, 11534352, 17367079, 26214472,
			40435818, 31588424, 25755698, 22806567, 22806567, 25755698, 31588424, 40435818,
		},
		// black bishop
		{
			2883600, 3538976, 3538976, 2883600, 2883600, 3538976, 3538976, 2883600,
			4325408, -2097168, -3538976, -1441808, -1441808, -3538976, -2097168, 4325408,
			4980768, -2883600, -4325408, -3538976, -3538976, -4325408, -2883600, 4980768,
			4980768, 0, -2883600, -6422576, -6422576, -2883600, 0, 4980768,
			4325408, -524288, -3538976, -7340080, -7340080, -3538976, -524288, 4325408,
			4325408, -2097168, -3538976, -2883600, -2883600, -3538976, -2097168, 4325408,
			3538976, -1441808, -1441808, 524288, 524288, -1441808, -1441808, 3538976,
			2097168, 4325408, 5767216, 6422576, 6422576, 5767216, 4325408, 2097168,
		},
		// black rook
		{
			3866691, -589757, -5177277, -9437117, -9437117, -5177277, -589757, 3866691,
			4063211, -393237, -4980757, -9240597, -7929877, -4980757, -393237, 4063211,
			5373904, 786384, -3538992, -7929904, -7929904, -3538992, 786384, 5373904,
			6619097, 1769433, -2424871, -6684711, -6684711, -2424871, 1769433, 6619097,
			6160362, 1376234, -2818070, -7077910, -7077910, -2818070, 1376234, 6160362,
			5570540, 786412, -3407892, -7733268, -7733268, -3407892, 786412, 5570540,
			4784119, -65545, -4259849, -8519689, -8519689, -4259849, -65545, 4784119,
			3735563, -1114101, -5308405, -9568245, -9568245, -5308405, -1114101, 3735563,
		},
		// black queen
		{
			1441856, 1441893, 1441919, 1441932, 1441932, 1441919, 1441893, 1441856,
			1900573, -786438, -786413, -786400, -786400, -786413, -786438, 1900573,
			2293766, -393244, -2490431, -2490419, -2490419, -2490431, -393244, 2293766,
			2686973, -38, -2097224, -3932270, -3932270, -2097224, -38, 2686973,
			3080192, 393181, -1704005, -3539051, -3539051, -1704005, 393181, 3080192,
			3473424, 786413, -1310774, -1310761, -1310761, -1310774, 786413, 3473424,
			3932205, 1245193, 1245219, 1245232, 1245232, 1245219, 1245193, 3932205,
			4325461, 4325500, 4325525, 4325538, 4325538, 4325525, 4325500, 4325461,
		},
		{
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0
		}
	};
}
