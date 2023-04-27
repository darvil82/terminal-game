export module render.sequences;

import <string>;

export class IRenderSequence {
	virtual std::wstring get_sequence() const = 0;
};
