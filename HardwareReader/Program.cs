using LibreHardwareMonitor.Hardware;
using System;
using System.Threading;

public class UpdateVisitor : IVisitor
{
    public void VisitComputer(IComputer computer) => computer.Traverse(this);
    public void VisitHardware(IHardware hardware)
    {
        hardware.Update();
        foreach (IHardware subHardware in hardware.SubHardware)
            subHardware.Accept(this);
    }
    public void VisitSensor(ISensor sensor) { }
    public void VisitParameter(IParameter parameter) { }
}

public class Program
{
    public static void Main()
    {
        var computer = new Computer
        {
            IsCpuEnabled = true,
            IsGpuEnabled = true,
            IsMemoryEnabled = true
        };
        computer.Open();
        Thread.Sleep(300);
        var visitor = new UpdateVisitor();

        while (true)
        {
            float ramUsed = 0, ramTotal = 0;

            computer.Accept(visitor);
            Thread.Sleep(150);

            foreach (IHardware hardware in computer.Hardware)
{
    hardware.Update();
    Thread.Sleep(50);

    // Check CPU sensors
    if (hardware.HardwareType == HardwareType.Cpu)
    {
        foreach (ISensor sensor in hardware.Sensors)
        {
            if (sensor.Value == null || float.IsNaN(sensor.Value.Value)) continue;

            if (sensor.SensorType == SensorType.Load && sensor.Name == "CPU Total")
                Console.WriteLine($"CPU_USAGE={Math.Round(sensor.Value.Value, 1)}");

            if (sensor.SensorType == SensorType.Temperature &&
                (sensor.Name.Contains("CPU") || sensor.Name.Contains("Tctl/Tdie")))
            {
                if (sensor.Value.Value > 1.0f)
                    Console.WriteLine($"CPU_TEMP={sensor.Value.Value:F1}");
                else
                    Console.WriteLine("CPU_TEMP=N/A");
            }
        }
    }
    // Check GPU sensors
    else if (hardware.HardwareType == HardwareType.GpuNvidia || hardware.HardwareType == HardwareType.GpuAmd || hardware.HardwareType == HardwareType.GpuIntel)
    {
        foreach (ISensor sensor in hardware.Sensors)
        {
            if (sensor.Value == null || float.IsNaN(sensor.Value.Value)) continue;

            if (sensor.SensorType == SensorType.Temperature && sensor.Name.ToLower().Contains("hot spot"))
                Console.WriteLine($"GPU_TEMP={sensor.Value.Value:F1}");

            if (sensor.SensorType == SensorType.Load && sensor.Name.ToLower().Contains("3d"))
                Console.WriteLine($"GPU_USAGE={sensor.Value.Value:F1}");
        }
    }
    // RAM sensors
    foreach (ISensor sensor in hardware.Sensors)
    {
        if (sensor.Name == "Memory Used")
            ramUsed = sensor.Value.Value;
        if (sensor.Name == "Memory")
            ramTotal = sensor.Value.Value;
    }
}


            if (ramTotal > 0)
                Console.WriteLine($"RAM_USAGE={(ramUsed / ramTotal * 100):F2}");

            Console.WriteLine("END_FRAME");
            Thread.Sleep(1000);
        }
    }
}
