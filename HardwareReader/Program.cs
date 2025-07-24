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
            IsMemoryEnabled = true,
            IsStorageEnabled = true
        };
        computer.Open();
        Thread.Sleep(300);
        var visitor = new UpdateVisitor();

        while (true)
{
    computer.Accept(visitor);
    Thread.Sleep(150);

    float? cpuTemp = null;
    float? cpuUsage = null;
    float? gpuTemp = null;
    float maxGpuUsage = -1f;
    float? ramUsage = null;
    float maxDiskUsage = -1f;

    foreach (IHardware hardware in computer.Hardware)
    {
        hardware.Update();
        Thread.Sleep(50);

        foreach (ISensor sensor in hardware.Sensors)
        {
            if (sensor.Value == null || float.IsNaN(sensor.Value.Value)) continue;

            string sensorName = sensor.Name.ToLower();

            switch (hardware.HardwareType)
            {
                case HardwareType.Cpu:
                    if (sensor.SensorType == SensorType.Load && sensor.Name == "CPU Total")
    cpuUsage = (float)Math.Round(sensor.Value.Value, 1);


                    if (sensor.SensorType == SensorType.Temperature &&
                        (sensor.Name.Contains("CPU") || sensor.Name.Contains("Tctl/Tdie")))
                        cpuTemp = sensor.Value.Value;
                    break;

                case HardwareType.Memory:
                    if (sensor.SensorType == SensorType.Load && sensor.Name == "Memory")
                        ramUsage = sensor.Value.Value;
                    break;

                case HardwareType.GpuAmd:
                case HardwareType.GpuIntel:
                case HardwareType.GpuNvidia:
                    if (sensor.SensorType == SensorType.Temperature &&
                        sensorName.Contains("hot spot") &&
                        gpuTemp == null)
                        gpuTemp = sensor.Value.Value;

                    if (sensor.SensorType == SensorType.Load && sensorName.Contains("3d"))
                        if (sensor.Value.Value > maxGpuUsage)
                            maxGpuUsage = sensor.Value.Value;
                    break;

                case HardwareType.Storage:
                    if (sensor.SensorType == SensorType.Load &&
                        (sensorName.Contains("usage") || sensorName.Contains("activity") || sensorName.Contains("total")))
                        if (sensor.Value.Value > maxDiskUsage)
                            maxDiskUsage = sensor.Value.Value;
                    break;
            }
        }
    }

    // ✅ Now print each value once
    if (cpuUsage.HasValue)
        Console.WriteLine($"CPU_USAGE={cpuUsage.Value:F1}");
    if (cpuTemp.HasValue)
        Console.WriteLine($"CPU_TEMP={cpuTemp.Value:F1}");
    if (gpuTemp.HasValue)
        Console.WriteLine($"GPU_TEMP={gpuTemp.Value:F1}");
    if (maxGpuUsage >= 0)
        Console.WriteLine($"GPU_USAGE={maxGpuUsage:F1}");
    if (ramUsage.HasValue)
        Console.WriteLine($"RAM_USAGE={ramUsage.Value:F1}");
    if (maxDiskUsage >= 0)
        Console.WriteLine($"DISK_USAGE={maxDiskUsage:F1}");

    Console.WriteLine("END_FRAME");
    Thread.Sleep(1000);
}

    }
}
