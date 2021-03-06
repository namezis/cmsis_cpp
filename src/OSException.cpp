#include "OSException.h"
#include <sstream>
#include "cmsis_os2.h"
#include "rtx_os.h"

namespace
{
	class cmsis_os_error_category : public std::error_category
	{
	public:
		cmsis_os_error_category(const cmsis_os_error_category&) = delete;
		constexpr cmsis_os_error_category() = default;
		virtual ~cmsis_os_error_category() = default;

		virtual const char* name() const noexcept override { return "cmsis os"; }

		virtual std::string message(int condition) const noexcept override
		{
			switch (condition)
			{
			case osOK            : return "Operation completed successfully";
			case osError         : return "Unspecified RTOS error: run-time error but no other error message fits";
			case osErrorTimeout  : return "Operation not completed within the timeout period";
			case osErrorResource : return "Resource not available";
			case osErrorParameter: return "Parameter error";
			case osErrorNoMemory : return "System is out of memory: it was impossible to allocate or reserve memory for the operation";
			case osErrorISR      : return "Not allowed in ISR context: the function cannot be called from interrupt service routines";
			case osStatusReserved: return "Prevents enum down-size compiler optimization";
			case osRtxErrorStackUnderflow: return "Stack underflow detected for thread";
			case osRtxErrorISRQueueOverflow: return "ISR Queue overflow detected when inserting object";
			case osRtxErrorTimerQueueOverflow: return "User Timer Callback Queue overflow detected for timer";
			case osRtxErrorClibSpace: return "Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM";
			case osRtxErrorClibMutex: return "Standard C/C++ library mutex initialization failed";

			default : return "Unknown error";
			}
		}
	};

	class cmsis_flag_error_category : public std::error_category
	{
	public:
		cmsis_flag_error_category(const cmsis_flag_error_category&) = delete;
		constexpr cmsis_flag_error_category() = default;
		virtual ~cmsis_flag_error_category() = default;

		virtual const char* name() const noexcept override { return "cmsis flag"; }

		virtual std::string message(int condition) const noexcept override
		{
			switch (condition)
			{
			case osFlagsErrorUnknown  : return "Generic error";
			case osFlagsErrorTimeout  : return "A timeout was specified and the specified flags were not set, when the timeout occurred";
			case osFlagsErrorResource : return "Try to get a flag that was not set and timeout 0 was specified, or the specified object identifier is corrupt or invalid";
			case osFlagsErrorParameter: return "A given parameter is wrong";
			case osFlagsErrorISR      : return "Not allowed in ISR context: the function cannot be called from interrupt service routines";

			default : return "Unknown error";
			}
		}
	};
}

namespace cmsis
{
	const std::error_category& os_category()
	{
		static cmsis_os_error_category osErrorCategory;
		return osErrorCategory;
	}

	const std::error_category& flags_category()
	{
		static cmsis_flag_error_category osErrorCategory;
		return osErrorCategory;
	}

	namespace internal
	{
		std::string str_error(const std::string& func, const void* id)
		{
			std::ostringstream oss;
			oss << func << '(' << id << ')';
			return oss.str();
		}
	}
}
