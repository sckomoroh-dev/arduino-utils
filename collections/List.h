#ifndef LIST_H
#define LIST_H

#include <cstdint>

namespace collections {
namespace {
    template<class TItem>
    struct ListItem {
        explicit ListItem(TItem* item)
            : ListItem(item, nullptr)
        {
        }

        ListItem(TItem* item, ListItem<TItem>* prev)
            : ListItem(item, prev, nullptr)
        {
        }

        ListItem(TItem* item, ListItem<TItem>* prev, ListItem<TItem>* next)
            : mItem(item)
            , mNext(next)
            , mPrev(prev)
        {

        }

        ~ListItem() {
            delete mItem;
        }

        TItem* mItem;
        ListItem<TItem>* mNext;
        ListItem<TItem>* mPrev;
    };
}

    template<class TItem>
    class List final {
    private:
        using ItemPtr = ListItem<TItem>*;

        ItemPtr mFirst;

    public:
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

        void add(TItem* item) {
            if (mFirst == nullptr) {
                mFirst = new ListItem<TItem>(item);
                return;
            }

            ItemPtr last = last();
            ItemPtr newItem = new ItemPtr(item, last);
            last->mNext = newItem;
        }

        void insertAfter(TItem* item) {
            ItemPtr current = mFirst;
            if (current == nullptr) {
                mFirst = new ItemPtr(item);
                return;
            }

            ItemPtr prev = nullptr;
            while (current != nullptr && current->mItem != item) {
                prev = current->mPrev;
                current = current->mNext;
            }

            ItemPtr newItem;
            if (current == nullptr) {
                newItem = new ItemPtr(item, prev);
                return;
            }

            newItem = new ItemPtr(item, current, current->mNext);
            current->mNext = newItem;
            if (current->mNext != nullptr) {
                current->mNext->mPrev = newItem;
            }
        }

        void remove(TItem* item) {
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

        uint8_t size() {
            uint8_t index = 0;
            ItemPtr current = mFirst;
            while (current != nullptr) {
                current = current->mNext;
                index++;
            }

            return index;
        }

        TItem* get(uint8_t index) {
            ItemPtr current = mFirst;
            while (current != nullptr && index-- > 0) {
                current = current->mNext;
            }

            if (current == nullptr) {
                return nullptr;
            }

            return current->mItem;
        }

        TItem* first() {
            if (mFirst == nullptr) {
                return nullptr;
            }

            return mFirst->mItem;
        }

        TItem* last() {
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

            return current->mItem;
        }

        TItem* next(TItem* item) {
            ItemPtr current = mFirst;
            if (current == nullptr) {
                return nullptr;
            }

            while (current != nullptr && current->mItem != item) {
                current = current->mNext;
            }

            if (current == nullptr || current->mNext == nullptr) {
                return nullptr;
            }


            return current->mNext->mItem;
        }

        TItem* prev(TItem* item) {
            ItemPtr current = mFirst;
            if (current == nullptr) {
                return nullptr;
            }

            while (current != nullptr && current->mItem != item) {
                current = current->mNext;
            }

            if (current == nullptr || current->mPrev == nullptr) {
                return nullptr;
            }

            return current->mPrev->mItem;
        }
    };
}

#endif // LIST_H