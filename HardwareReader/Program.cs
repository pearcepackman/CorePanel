using LibreHardwareMonitor.Hardware;
using System;
using System.IO;

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
        string tempPath = Path.Combine(Path.GetTempPath(), "corepanel_metrics.txt");
        using StreamWriter writer = new StreamWriter(tempPath, false);
        float ramUsed = 0;
        float ramTotal = 0;

        var computer = new Computer
        {
            IsCpuEnabled = true,
            IsGpuEnabled = true,
            IsMemoryEnabled = true
        };

        computer.Open();
        computer.Accept(new UpdateVisitor());

        foreach (IHardware hardware in computer.Hardware)
        {
            hardware.Update();

            foreach (ISensor sensor in hardware.Sensors)
            {
                if (sensor.Value == null || float.IsNaN(sensor.Value.Value)) continue;

                // CPU Usage
                if (sensor.SensorType == SensorType.Load && sensor.Name == "CPU Total") {
                    writer.WriteLine($"CPU_USAGE={sensor.Value}");
                    Console.WriteLine($"CPU_USAGE={sensor.Value}");
                }

                // CPU Temp
                if (sensor.SensorType == SensorType.Temperature && sensor.Name == "CPU Core Max")
                {
                    writer.WriteLine($"CPU_TEMP={sensor.Value}");
                    Console.WriteLine($"CPU_TEMP={sensor.Value}");
                }
                // RAM
                if (sensor.Name == "Memory Used")
                    ramUsed = sensor.Value.Value;
                if (sensor.Name == "Memory")
                    ramTotal = sensor.Value.Value;

                // GPU Temp (RX 7600)
                if (hardware.Name.Contains("RX 7600") && sensor.Name == "GPU Hot Spot")
                    writer.WriteLine($"GPU_TEMP={sensor.Value}");

                // GPU Usage
                if (hardware.Name.Contains("RX 7600") && sensor.Name == "D3D 3D")
                    writer.WriteLine($"GPU_USAGE={sensor.Value}");

            }
        }

        if (ramTotal > 0)
            writer.WriteLine($"RAM_USAGE={(ramUsed / ramTotal * 100):F2}");

        computer.Close();
    }
}
