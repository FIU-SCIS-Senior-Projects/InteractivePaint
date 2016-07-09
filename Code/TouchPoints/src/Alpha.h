#pragma once

namespace touchpoints { namespace drawing
{
	class Alpha
	{
	public:
		Alpha();
		Alpha(float value);
		inline float GetValue() { return alpha; }
		void SetValue(float value);
	private:
		float alpha;
	};
}}