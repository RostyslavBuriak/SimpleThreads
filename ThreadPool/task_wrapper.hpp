#ifndef TASK_WRAPPER_HPP
#define TASK_WRAPPER_HPP
class task_wrapper{
    public:
        template<typename T>
        task_wrapper(T&& f):func(new impl<T>(std::move(f))){};

        task_wrapper() = default;
        task_wrapper(const task_wrapper&) = delete;
        task_wrapper(task_wrapper&& t):func(std::move(t.func)){}

        task_wrapper& operator=(const task_wrapper&) = delete;
        task_wrapper& operator=(task_wrapper&& t){
            func = std::move(t.func);
            return *this;
        }

        void operator()(){(*func)();}
    private:
        struct impl_base
        {
            virtual void operator()() = 0;
            virtual ~impl_base() = default;
        };

        template<typename T>
        struct impl : impl_base
        {
            T f;
            impl(T&& _f):f(std::forward<T>(_f)){}
            virtual void operator()(){f();}
        };
        
        std::unique_ptr<impl_base> func;
        
};
#endif