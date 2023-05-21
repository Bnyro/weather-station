let calendar;

let $ = document.querySelector.bind(document);
let $$ = document.querySelectorAll.bind(document);

const styledConfirm = (title, message, callback) => {
  let $dialog = $("#confirmdialog").querySelector.bind($("#confirmdialog"));
  $dialog("#title").innerHTML = title;
  $dialog("#message").innerHTML = message;
  $dialog("#cancel").addEventListener("click", () => {
    $("#confirmdialog").close();
  });
  $dialog("#confirm").addEventListener("click", () => {
    callback();
    $("#confirmdialog").close();
  });
  $("#confirmdialog").showModal();
};

const dynamicColor = () => {
  return "#" + ((Math.random() * 0xffffff) << 0).toString(16);
};

const removeEvent = async (event) => {
  styledConfirm("Delete", 'Delete "' + event.title + '"?', async function () {
    await fetch("/api/events", {
      method: "DELETE",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        id: parseInt(event.id),
      }),
    });
    event.remove();
  });
};

const createEvent = async (info) => {
  $("#startdate").value = info.dateStr.split("T")[0] || info.dateStr;
  $("#starttime").value = info.dateStr.split("T")[1]?.split("+")[0] || "12:00";
  $("#color").value = dynamicColor();
  $("#create").showModal();
};

document.addEventListener("DOMContentLoaded", () => {
  var container = document.querySelector("FullCalendar");
  calendar = new FullCalendar.Calendar(container, {
    headerToolbar: {
      left: "prev,next today",
      center: "title",
      right: "dayGridMonth,timeGridWeek,timeGridDay,listYear",
    },
    themeSystem: "bootstrap5",
    initialView: "dayGridMonth",
    dateClick: (info) => createEvent(info),
    eventClick: (info) => removeEvent(info.event),
    events: "/api/events",
  });
  calendar.render();
});

$("#cancel").addEventListener("click", () => {
  $("#create").close();
});

$("#create>form").addEventListener("submit", async () => {
  let offset = new Date().getTimezoneOffset() / -60;
  if ((offset.length = 1)) offset = "0" + offset;
  const event = {
    id: Date.now(),
    title: $("#title").value,
    start:
      $("#startdate").value +
      "T" +
      $("#starttime").value +
      ":00+" +
      offset +
      ":00",
    end:
      $("#enddate").value + "T" + $("#endtime").value + ":00+" + offset + ":00",
    color: $("#color").value,
  };
  await fetch("/api/events", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(event),
  });
  calendar.addEvent(event);
  $("#create>form").reset();
});

$("#enddate").addEventListener("change", () => {
  $("#endtime").required = $("#enddate").value;
});

$("#endtime").addEventListener("change", () => {
  $("#enddate").required = $("#endtime").value;
});
