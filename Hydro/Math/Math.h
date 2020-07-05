#ifndef MATH_H
#define MATH_H

#include <limits>

namespace Hydro{
	class Math{
	public:
		static inline constexpr float PI(){
			return 3.141592653589793238f;
		};

		static inline constexpr float ConvertToRadians(const float angle_){
			return angle_ * (PI() / 180.0f);
		}

		static inline constexpr float ConvertToDegrees(const float angle_){
			return angle_ * (180.0f / PI());
		}

		static inline float Infinity(){
			return std::numeric_limits<float>::infinity();
		}

		static inline float Pow(float base, int exponent){
			float number = base;
			if(exponent < 0){
				for(int i = 0; i >= exponent; i--){
					number /= base;
				}
			}else if(exponent > 0){
				for(int i = 1; i < exponent; i++){
					number *= base;
				}
			}else{
				number = 1;
			}

			return number;
		}

		static inline float Sqrt(float value_){
			//Up-converts argument to double because double sqrt is 5x faster than the float version for some reason
			return static_cast<float>(sqrt(static_cast<double>(value_)));
		}

		template <class T>
		static inline constexpr T Clamp(T min_, T max_, T value_){
			if(value_ < min_){
				return min_;
			}else if(value_ > max_){
				return max_;
			}

			return value_;
		}

		static inline constexpr float NearZero(){
			return 1e-7f;
		};

		static inline constexpr bool NearZero(const float s){
			if(s <= NearZero() && s >= -NearZero()){
				return true;
			}else{
				return false;
			}
		}

		static inline constexpr bool Near(const float a, const float b){
			const float diff = a - b;
			if(NearZero(diff)){
				return true;
			}else{
				return false;
			}
		}

		static inline float Sin(float angle_){
			return static_cast<float>(sin(ConvertToRadians(angle_)));
		}

		static inline float Cos(float angle_){
			return static_cast<float>(cos(ConvertToRadians(angle_)));
		}

		static inline float Tan(float angle_){
			return static_cast<float>(tan(ConvertToRadians(angle_)));
		}

		static inline float Asin(float sin_){
			return ConvertToDegrees(static_cast<float>(asin(sin_)));
		}

		static inline float Acos(float cos_){
			return ConvertToDegrees(static_cast<float>(acos(Clamp(-1.0f, 1.0f, cos_))));
		}

		static inline float Atan(float tan_){
			return ConvertToDegrees(static_cast<float>(atan(tan_)));
		}

		static inline float Atan2(float a_, float b_){
			return ConvertToDegrees(static_cast<float>(atan2(a_, b_)));
		}

		template <class T>
		static inline T Abs(T value_){
			return abs(value_);
		}

		//Delete unwanted compiler-generated constructors, destructors, and assignment operators
		Math() = delete;
		Math(const Math&) = delete;
		Math(Math&&) = delete;
		Math& operator=(const Math&) = delete;
		Math& operator=(Math&&) = delete;
		~Math() = delete;
	};
}

#endif //!MATH_H