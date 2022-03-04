#pragma once

#include"../base.h"

namespace FF::Wrapper {
	class Instance {

	public:

		using Ptr = std::shared_ptr<Instance>;
		static Ptr create() { return std::make_shared<Instance>(); }

		Instance();

		~Instance();

	private:

		VkInstance mInstance;
	};
};