#ifndef __lcpp_laenv_h
#define __lcpp_laenv_h

#include <string>

namespace LCPP{
	
	class LAENV {
	public:

		enum SPEC {
			Optimal=1,
			Minimum=2,
			CrossOver=3,
			MinimumColumn = 5,
			CrossOverSVD=6

		};
		
		LAENV(){}

		int operator()(SPEC spec, const std::string& name, const std::string& opts, int n1, int n2, int n3, int n4);
	};

	
}

#endif
