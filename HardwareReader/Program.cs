using LibreHardwareMonitor.Hardware;
using System;
using System.IO;
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
        float ramUsed = 0;
        float ramTotal = 0;



        // Second pass to get updated values
            computer.Accept(visitor);
            Thread.Sleep(150);

        foreach (IHardware hardware in computer.Hardware)
            {
                hardware.Update();
                Thread.Sleep(50);

                foreach (ISensor sensor in hardware.Sensors)
                {
                    if (sensor.Value == null || float.IsNaN(sensor.Value.Value)) continue;

                    if (sensor.SensorType == SensorType.Load && sensor.Name == "CPU Total")
                    {
                        if (sensor.Value.HasValue)
                        {
                            float roundedCpu = (float)Math.Round(sensor.Value.Value, 1);
                            Console.WriteLine($"CPU_USAGE={roundedCpu}");
                        }
                    }


                    if (sensor.SensorType == SensorType.Temperature &&
                        (sensor.Name.Contains("CPU") || sensor.Name.Contains("Tctl/Tdie")))
                    {
                        if (sensor.Value != null && !float.IsNaN(sensor.Value.Value) && sensor.Value.Value > 1.0f) {
                            Console.WriteLine($"CPU_TEMP={sensor.Value.Value:F1}");
                        } else {
                            Console.WriteLine("CPU_TEMP=N/A");
                        }
                    }



                    if (sensor.Name == "Memory Used") ramUsed = sensor.Value.Value;
                    if (sensor.Name == "Memory") ramTotal = sensor.Value.Value;

                    if (hardware.Name.Contains("RX 7600") && sensor.Name == "GPU Hot Spot")
                        Console.WriteLine($"GPU_TEMP={sensor.Value}");

                    if (hardware.Name.Contains("RX 7600") && sensor.Name == "D3D 3D")
                        Console.WriteLine($"GPU_USAGE={sensor.Value}");
                }
            }

        if (ramTotal > 0)
            Console.WriteLine($"RAM_USAGE={(ramUsed / ramTotal * 100):F2}");

        Console.WriteLine("END_FRAME"); // used to delimit between readings

        Thread.Sleep(1000); // delay before next reading
    }

    // no close, runs forever
}


}
