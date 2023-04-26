import <concepts>;

export module utils.concepts;


export template<class C, class B>
concept Extends = std::is_base_of_v<B, C>;

