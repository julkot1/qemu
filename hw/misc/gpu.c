#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/units.h"
#include "hw/pci/pci.h"
#include "hw/hw.h"
#include "hw/pci/msi.h"
#include "qemu/timer.h"
#include "qom/object.h"
#include "qemu/main-loop.h"
#include "qemu/module.h"
#include "qapi/visitor.h"
#define TYPE_PCI_GPU_DEVICE "AREK"
#define GPU_DEVICE_ID 0x2137
#define PCI_VENDOR_ID_CUSTOM 0x6969

typedef struct GpuState {
    PCIDevice pdev;
    MemoryRegion mmio;  // BAR0
} GpuState;

DECLARE_INSTANCE_CHECKER(GpuState, GPU, TYPE_PCI_GPU_DEVICE)

static void pci_gpu_register_types(void);
static void gpu_instance_init(Object *obj);
static void gpu_class_init(ObjectClass *class, const void *data);
static void pci_gpu_realize(PCIDevice *pdev, Error **errp);
static void pci_gpu_uninit(PCIDevice *pdev);

type_init(pci_gpu_register_types)

/* MMIO callbacks */
static uint64_t gpu_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    printf("MMIO Read: addr=0x%llx size=%u\n", (unsigned long long)addr, size);
    return 0;
}

static void gpu_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    printf("MMIO Write: addr=0x%llx val=0x%llx size=%u\n",
           (unsigned long long)addr, (unsigned long long)val, size);
}

static const MemoryRegionOps gpu_mmio_ops = {
    .read = gpu_mmio_read,
    .write = gpu_mmio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

/* Device registration */
static void pci_gpu_register_types(void)
{
    static InterfaceInfo interfaces[] = {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    };

    static const TypeInfo gpu_info = {
        .name = TYPE_PCI_GPU_DEVICE,
        .parent = TYPE_PCI_DEVICE,
        .instance_size = sizeof(GpuState),
        .instance_init = gpu_instance_init,
        .class_init = gpu_class_init,
        .interfaces = interfaces,
    };

    type_register_static(&gpu_info);
}

static void gpu_instance_init(Object *obj)
{
    printf("GPU instance init\n");
}

static void gpu_class_init(ObjectClass *class, const void *data)
{
    printf("Class init\n");

    PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

    k->realize    = pci_gpu_realize;
    k->exit       = pci_gpu_uninit;
    k->vendor_id  = PCI_VENDOR_ID_CUSTOM;
    k->device_id  = GPU_DEVICE_ID;
    k->revision   = 0x01;
    k->class_id   = PCI_CLASS_DISPLAY_OTHER;
}

static void pci_gpu_realize(PCIDevice *pdev, Error **errp)
{
    GpuState *s = GPU(pdev);
    printf("GPU Realize\n");

    #define GPU_MMIO_SIZE 0x04000000  

    memory_region_init_io(&s->mmio, OBJECT(s), &gpu_mmio_ops, s, "gpu-mmio", GPU_MMIO_SIZE);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);
}

static void pci_gpu_uninit(PCIDevice *pdev)
{
    printf("GPU un-init\n");
}
