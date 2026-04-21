module.exports = [
  {
    "type": "heading",
    "defaultValue": "Seasonal Watchface"
  },
  {
    "type": "text",
    "defaultValue": "Customize your watchface appearance, season, Bluetooth alerts, and animation."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Appearance"
      },
      {
        "type": "color",
        "messageKey": "TextColor",
        "defaultValue": "0xFFFFFF",
        "label": "Text Color"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Season"
      },
      {
        "type": "select",
        "messageKey": "SeasonMode",
        "defaultValue": "auto",
        "label": "Season Mode",
        "options": [
          { "label": "Auto", "value": "auto" },
          { "label": "Manual", "value": "manual" },
          { "label": "Daily Random", "value": "random" }
        ]
      },
      {
        "type": "select",
        "messageKey": "ManualSeason",
        "defaultValue": "autumn",
        "label": "Manual Season",
        "options": [
          { "label": "Spring", "value": "spring" },
          { "label": "Summer", "value": "summer" },
          { "label": "Autumn", "value": "autumn" },
          { "label": "Winter", "value": "winter" }
        ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Bluetooth"
      },
      {
        "type": "toggle",
        "messageKey": "ShowBtIcon",
        "label": "Show Icon When Disconnected",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "BtVibrate",
        "label": "Vibrate On Disconnect",
        "defaultValue": true
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Preferences"
      },
      {
        "type": "toggle",
        "messageKey": "ShowDate",
        "label": "Show Date",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "LowVisionMode",
        "label": "Low Vision Mode",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "AnimateLeaves",
        "label": "Animate Seasonal Effects",
        "defaultValue": true
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
