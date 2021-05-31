QString BBFileListWidget::lineFeed(QString originalText)
{
    // 40 corresponds to QSize(45, 90)
    QFontMetrics fm = fontMetrics();
    QString lineFeedText;
    int nHeadIndex = 0;
    for (int i = 0; i < originalText.length(); i++)
    {
        if (fm.width(originalText.mid(nHeadIndex, i + 1 - nHeadIndex)) < m_ItemSize.width() - 5)
        {
            // The width does not exceed 40, no need to line feed
            lineFeedText += originalText.at(i);
        }
        else
        {
            // Line feed
            // The index of the first character is set to the index of the first character of the next line
            lineFeedText += "\n" + originalText[i];
            nHeadIndex = i;
        }
    }
    return lineFeedText;
}