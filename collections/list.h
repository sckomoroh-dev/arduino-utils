#ifndef LIST_H
#define LIST_H

#define uint8_t unsigned char

namespace collections {

	template<class TItem>
    class List;

	template<class TItem>
	class ListIter final {
	private:
        TItem mItem;
        ListIter<TItem>* mNext = nullptr;
        ListIter<TItem>* mPrev = nullptr;
        bool mEmpty = false;

	private:
        explicit ListIter(const TItem& item)
            : mItem(item)
        {
        }

        ListIter()
        	: mEmpty(true)
		{
		}

	public:
		ListIter<TItem>* next() const {
			return ListIter(mItem->mNext, mPrev, mNext);
		}

		ListIter<TItem>* prev() const {
			return ListIter(mItem->mPrev, mPrev, mNext);
		}

		const TItem& value() const {
			return mItem;
		}

		friend class List<TItem>;
	};

    template<class TItem>
    class List final {
    private:
        using ItemPtr = ListIter<TItem>*;
        using Item = ListIter<TItem>;

        ItemPtr mFirst;

    public:
        static const ListIter<TItem>* empty;

        List()
            : mFirst(nullptr)
        {
        }

        ~List() {
            ItemPtr current = mFirst;
            ItemPtr next;
            while (current != nullptr) {
                next = current->mNext;
                delete current;
                current = next;
            }
        }

        void add(const TItem& item) {
            if (mFirst == nullptr) {
                mFirst = new Item(item);
                return;
            }

            ItemPtr lastItem = getLastItemPtr();
            ItemPtr newItem = new Item(item);
            newItem->mPrev = lastItem;
            lastItem->mNext = newItem;
        }

        void insertBefore(const TItem& item, const ListIter<TItem>* iter) {
            ItemPtr newItem;
            newItem = new Item(item);

            ItemPtr current = mFirst;
            if (current == nullptr) {
                mFirst = new Item(item);
                return;
            }

            ItemPtr prev = nullptr;
            while (current != nullptr && current != iter) {
                prev = current->mPrev;
                current = current->mNext;
            }

            if (current == nullptr) {
                newItem->mPrev = prev;
                return;
            }

            newItem->mNext = current;
            newItem->mPrev = prev;

            current->mPrev = newItem;

            if (newItem->mPrev != nullptr) {
            	newItem->mPrev->mNext = newItem;
            }

            if (newItem->mNext == mFirst) {
            	mFirst = newItem;
            }
        }

        void insertAfter(const TItem& item, const ListIter<TItem>* iter) {
            ItemPtr newItem;
            newItem = new Item(item);

            ItemPtr current = mFirst;
            if (current == nullptr) {
                mFirst = new Item(item);
                return;
            }

            ItemPtr prev = nullptr;
            while (current != nullptr && current != iter) {
                prev = current->mPrev;
                current = current->mNext;
            }

            if (current == nullptr) {
                newItem->mPrev = prev;
                return;
            }

            newItem->mPrev = current;
            newItem->mNext = current->mNext;

            current->mNext = newItem;

            if (newItem->mNext != nullptr) {
            	newItem->mNext->mPrev = newItem;
            }
        }

        void remove(const TItem& item) {
            ItemPtr current = mFirst;
            while (current != nullptr && current->mItem != item) {
                current = current->mNext;
            }

            if (current == nullptr) {
                return;
            }

            if (current->mPrev != nullptr) {
                current->mPrev->mNext = current->mNext;
            }

            if (current->mNext != nullptr) {
                current->mNext->mPrev = current->mPrev;
            }

            delete current;
        }

        uint8_t size() const {
            uint8_t index = 0;
            ItemPtr current = mFirst;
            while (current != nullptr) {
                current = current->mNext;
                index++;
            }

            return index;
        }

        const ListIter<TItem>* get(uint8_t index) const {
            ItemPtr current = mFirst;
            while (current != nullptr && index-- > 0) {
                current = current->mNext;
            }

            if (current == nullptr) {
                return empty;
            }

            return current;
        }

        const ListIter<TItem>* first() const {
            if (mFirst == nullptr) {
                return empty;
            }

            return mFirst;
        }

        const ListIter<TItem>* last() const {
        	ItemPtr lastItem = getLastItemPtr();
        	if (lastItem == nullptr) {
        		return empty;
        	}

        	return lastItem;
        }

    private:
        ItemPtr getLastItemPtr() const {
            ItemPtr current = mFirst;
            if (current == nullptr) {
                return nullptr;
            }

            while (current->mNext != nullptr) {
                current = current->mNext;
            }

            if (current == nullptr) {
                return nullptr;
            }

            return current;
        }
    };

    template<class TItem>
    const ListIter<TItem>* List<TItem>::empty = new ListIter<TItem>();
}


#endif // LIST_H
