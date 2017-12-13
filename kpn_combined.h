#ifndef KPN_COMBINED_H
#define KPN_COMBINED_H

#include <systemc.h>


SC_MODULE(kpn_combined)
{
    private:
    sc_fifo<uint32_t> fifo_a;
    sc_fifo<uint32_t> fifo_b;
    sc_fifo<uint32_t> fifo_c;
    sc_fifo<uint32_t> fifo_d;

    sc_signal<uint32_t> e_out;

    uint32_t counter;

    void delay(void)
    {
        while(true)
        {
            uint32_t d;

            wait(1,SC_NS);

            fifo_d.read(d);
            fifo_c.write(d);
        }
    }

    void split(void)
    {
        while(true)
        {
            uint32_t b;


            fifo_b.read(b);

            fifo_a.write(b);
            fifo_d.write(b);
            e_out.write(b);

            if (counter <= 10)
            {
                counter++;
                std::cout<<"Output: "<< b <<std::endl;
            }
            else
            {
                wait(); // stop simulation
            }
        }
    }

    void add(void)
    {
        while(true)
        {
            uint32_t a;
            uint32_t c;
            uint32_t sum;

            fifo_a.read(a);
            fifo_c.read(c);

            sum = a + c;

            fifo_b.write(sum);
        }
    }

    public:
    SC_CTOR(kpn_combined) : fifo_a("fifo_a", 10), fifo_b("fifo_b", 10), fifo_c("fifo_c", 10), fifo_d("fifo_d", 10)
    {
        counter = 0;
        uint32_t initial_b = 1;
        fifo_b.write(initial_b);

        uint32_t initial_c = 0;
        fifo_c.write(initial_c);

        SC_THREAD(add);
        SC_THREAD(split);
        SC_THREAD(delay);
    }
};

#endif // KPN_COMBINED_H
