#include <sys/mman.h>
#include <iostream>
// блок по 16
#include "memory_allocator.h"

class FSA final : public MemoryAllocator
{
public:
    struct Node
    {
        Node(Node* _next, void* _memory)
        {
            next = _next;
            memory = _memory;
        }

        Node * next;
        void * memory;
        size_t free_block_index;
    };

    FSA(size_t size_of_block)
    {
        // _size_of_block = size_of_block;
        _size_of_block = size_of_block;
        std::cout << "Node: " << sizeof(Node) << std::endl; 
        
    }

    ~FSA() override
    {
        destroy();
    }

    void init() override
    {
        _inited = true;
        _count_of_pages = 1;
        _count_of_blocks = PageSize / _size_of_block;
        if (_size_of_block == 16)
        {
            _count_of_blocks -= 2;
        }
        
        void* _map = mmap(NULL, PageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

        // _current_page = new Node(nullptr, _map);
        
        // добавить не два блока после 16
        void* node_place = (char*)_map;
        _current_page = new ((Node *)node_place) Node(nullptr, _map);
        _first_page = _current_page;

        for (size_t i = 0; i < _count_of_blocks; i++)
        {
            void* p = (char*)_current_page->memory + (i + 2) * _size_of_block;
            *((int*)p) = i + 1;
        }

        _current_page->free_block_index = 0;
        // _free_block_index = 1;
    }

    void destroy() override
    {
        if (_inited == true)
        {
            Node* temp = _first_page;

            // TODO не могу munmap в вайл 

            while (temp->next != nullptr)
            {
                std::cout << "In destroy while" << std::endl;
                std::cout << "Free_block index " << temp->free_block_index << std::endl;
                Node* temp_second = temp;
                temp = temp->next;
                munmap(temp_second->memory, PageSize);
            }
            // std::cout << "Free_block index " << _temp->free_block_index << std::endl;
            // munmap(_temp->memory, PageSize);
            // // Не перекидывается потому что чистим память раньше
            // _temp = _temp->next;

            std::cout << "Out_of_destroy_while" << std::endl;
            std::cout << "Free_block index " << temp->free_block_index << std::endl;
            munmap(temp->memory, PageSize);

            _first_page = nullptr;
            _current_page = nullptr;
            _size_of_block = 0;
            _count_of_blocks = 0;
            _count_of_pages = 0;
            _inited = false;
        }
        
    }

    void* alloc(size_t size) override
    {
        if (_current_page->free_block_index == _count_of_blocks)
        {
            if (_current_page->next == nullptr)
            {
                _count_of_pages++;
                void* temp = mmap(NULL, PageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

                Node* old_node_place = (Node*)_current_page->memory;
                void* node_place = (char*)temp;
                // _current_page->next = 
                _current_page->next = (Node *)node_place;
                _current_page = new ((Node *)node_place) Node(nullptr, temp);

                for (size_t i = 0; i < _count_of_blocks; i++)
                {
                    void* p = (char*)temp + (i + 2) * _size_of_block;
                    *((int*)p) = i + 1;
                }

                // _map = temp;

                _current_page->free_block_index = 0;
            }
            else
            {
                _current_page = _current_page->next;
                // _map = _current_page->memory;
            }
        }

        void* ptr = (char*)_current_page->memory + (_current_page->free_block_index + 2) * _size_of_block;
        _current_page->free_block_index = *((int*) ptr);
        std::cout << "free_block_index " << _current_page->free_block_index << std::endl;
        
        return ptr;
    }

    void free(void* p) override
    {
        Node * temp = _first_page;
        
        while (temp->next != nullptr)
        {
            for (size_t i = 0; i < _count_of_blocks; i++)
            {
                void* temp_mem = temp->memory;
                void* p_temp = (char*)temp_mem + (i + 2) * _size_of_block;
                if (p_temp == p)
                {
                    std::cout << "Блок совпадает" << std::endl;
                    *((int*)p) = temp->free_block_index;
                    temp->free_block_index = ((char*)p - (char*)temp->memory) / _size_of_block - 2;
                    if (_current_page->memory != temp->memory)
                    {
                        std::cout << "В проверке 1" << std::endl;
                        _current_page = temp;
                        // _map = temp_mem;
                    }
                }
            }

            temp = temp->next;
        }
        for (size_t i = 0; i < _count_of_blocks; i++)
        {
            void* temp_mem = temp->memory;
            void* p_temp = (char*)temp_mem + (i + 2) * _size_of_block;
            if (p_temp == p)
            {
                std::cout << "Блок совпадает" << std::endl;
                *((int*)p) = temp->free_block_index;
                temp->free_block_index = ((char*)p - (char*)temp->memory) / _size_of_block - 2;
                if (_current_page->memory != temp->memory)
                {
                    std::cout << "В проверке 2" << std::endl;
                    _current_page = temp;
                    // _map = temp_mem;
                }
            }
        }
    }

    bool contains(void* ptr) const
    {
        Node * temp = _first_page;
        for (size_t i = 0; i < _count_of_pages; i++)
        {
            for (size_t i = 0; i < _count_of_blocks; i++)
            {
                void* temp_mem = temp->memory;
                void* p_temp = (char*)temp_mem + (i + 2) * _size_of_block;
                if (p_temp == ptr)
                {
                    return true;
                }
            }
            temp = temp->next;
        }
        return false;
    }

    // Тест Метод
    void show_mapped()
    {
        void* p = (char*)_current_page->memory;
        Node* temp = (Node*)p;
        std::cout << "Node: " << temp->free_block_index << std::endl;

        for (size_t i = 0; i < _count_of_blocks; i++)
        {
            auto p = (char*)temp->memory + (i + 2)*_size_of_block;
            std::cout << "Index: " << *((int*)p) << std::endl;
        }
    }

private:
    // const static size_t PageSize = 128;
    size_t _size_of_block;
    size_t _count_of_blocks;
    size_t _count_of_pages;
    bool _inited;
    Node* _current_page;
    Node* _first_page;
};