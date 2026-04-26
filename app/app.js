const SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

let bleDevice;
let writeCharacteristic;

const connectBtn = document.getElementById('connectBtn');
const statusText = document.getElementById('statusText');
const controls = document.getElementById('controls');
const syncTimeBtn = document.getElementById('syncTimeBtn');

connectBtn.addEventListener('click', async () => {
    try {
        statusText.innerText = "Scanning...";
        bleDevice = await navigator.bluetooth.requestDevice({
            filters: [{ name: 'Restomod_Clock' }],
            optionalServices: [SERVICE_UUID]
        });
        statusText.innerText = "Connecting...";
        const server = await bleDevice.gatt.connect();
        const service = await server.getPrimaryService(SERVICE_UUID);
        writeCharacteristic = await service.getCharacteristic(CHARACTERISTIC_UUID);
        await writeCharacteristic.startNotifications();
        writeCharacteristic.addEventListener('characteristicvaluechanged', handleNotifications);
        statusText.innerText = "ONLINE";
        controls.classList.remove('disabled');
    } catch (error) {
        console.error(error);
        statusText.innerText = "Error";
    }
});

function handleNotifications(event) {
    const value = new TextDecoder().decode(event.target.value);
    if (value.startsWith("D:")) {
        const parts = value.substring(2).split(",");
        document.getElementById('telemetryData').innerHTML = `
            <div class="tel-item"><div class="tel-val">${parts[0]}</div><div class="tel-lbl">Altitud</div></div>
            <div class="tel-item"><div class="tel-val">${parts[1]}</div><div class="tel-lbl">Temp</div></div>
            <div class="tel-item"><div class="tel-val">${parts[2]}</div><div class="tel-lbl">G-Force</div></div>
        `;
    }
}

async function sendCommand(cmd) {
    if (!writeCharacteristic) return;
    await writeCharacteristic.writeValue(new TextEncoder().encode(cmd));
}

syncTimeBtn.addEventListener('click', () => {
    const now = new Date();
    const cmd = `T:${now.getUTCFullYear()},${now.getUTCMonth()+1},${now.getUTCDate()},${now.getUTCHours()},${now.getUTCMinutes()},${now.getUTCSeconds()}`;
    sendCommand(cmd);
});

document.getElementById('applyFontBtn').addEventListener('click', () => {
    sendCommand(`F:${document.getElementById('leftWidget').value}`);
    const theme = document.getElementById('themeSelect').value;
    if (theme !== "-1") sendCommand(`M:${theme}`);
});

document.getElementById('bgModeSelect').addEventListener('change', (e) => {
    sendCommand(`G:${e.target.value}`);
});
